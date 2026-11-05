/*******************************************************************************
 * @file    main.c
 * @brief   Barebones Pico-ROS node for the Waveshare ESP32-P4-NANO (Ethernet)
 *
 * @details Brings up the on-board IP101 Ethernet PHY with DHCP, connects to a
 *          ROS 2 rmw_zenohd router as a zenoh client and runs:
 *            - talker:   std_msgs/String on  /picoros/chatter  (1 Hz)
 *            - listener: std_msgs/String on  /picoros/listener (printed)
 *
 *          Router locator, node name and domain are set in menuconfig
 *          ("Pico-ROS example").
 *******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "esp_eth.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "picoros.h"
#include "picoserdes.h"

static const char *TAG = "picoros_p4";

/* --- Ethernet (ESP32-P4-NANO: IP101 PHY over RMII) ------------------------ */

#define ETH_PHY_ADDR      1
#define ETH_PHY_RST_GPIO  51
#define ETH_MDC_GPIO      31
#define ETH_MDIO_GPIO     52

#define GOT_IP_BIT BIT0
static EventGroupHandle_t s_net_events;

static void eth_event(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    if (id == ETHERNET_EVENT_CONNECTED) {
        ESP_LOGI(TAG, "ethernet link up");
    } else if (id == ETHERNET_EVENT_DISCONNECTED) {
        ESP_LOGW(TAG, "ethernet link down");
        xEventGroupClearBits(s_net_events, GOT_IP_BIT);
    }
}

static void ip_event(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    const ip_event_got_ip_t *ev = (const ip_event_got_ip_t *)data;
    ESP_LOGI(TAG, "got ip " IPSTR " gw " IPSTR, IP2STR(&ev->ip_info.ip), IP2STR(&ev->ip_info.gw));
    xEventGroupSetBits(s_net_events, GOT_IP_BIT);
}

static void ethernet_start(void)
{
    s_net_events = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_config_t netif_cfg = ESP_NETIF_DEFAULT_ETH();
    esp_netif_t *netif = esp_netif_new(&netif_cfg);

    eth_mac_config_t mac_cfg = ETH_MAC_DEFAULT_CONFIG();
    eth_phy_config_t phy_cfg = ETH_PHY_DEFAULT_CONFIG();
    phy_cfg.phy_addr = ETH_PHY_ADDR;
    phy_cfg.reset_gpio_num = ETH_PHY_RST_GPIO;

    eth_esp32_emac_config_t emac_cfg = ETH_ESP32_EMAC_DEFAULT_CONFIG();
    emac_cfg.smi_gpio.mdc_num = ETH_MDC_GPIO;
    emac_cfg.smi_gpio.mdio_num = ETH_MDIO_GPIO;

    esp_eth_mac_t *mac = esp_eth_mac_new_esp32(&emac_cfg, &mac_cfg);
    esp_eth_phy_t *phy = esp_eth_phy_new_ip101(&phy_cfg);

    esp_eth_config_t eth_cfg = ETH_DEFAULT_CONFIG(mac, phy);
    esp_eth_handle_t eth = NULL;
    ESP_ERROR_CHECK(esp_eth_driver_install(&eth_cfg, &eth));
    ESP_ERROR_CHECK(esp_netif_attach(netif, esp_eth_new_netif_glue(eth)));
    ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &ip_event, NULL));
    ESP_ERROR_CHECK(esp_eth_start(eth));

    ESP_LOGI(TAG, "waiting for DHCP...");
    xEventGroupWaitBits(s_net_events, GOT_IP_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
}

/* --- Pico-ROS ------------------------------------------------------------- */

static void listener_cb(uint8_t *rx_data, size_t data_len);

static picoros_node_t node = {
    .name = CONFIG_PICOROS_NODE_NAME,
    .domain_id = CONFIG_PICOROS_DOMAIN_ID,
};

static picoros_publisher_t pub_chatter = {
    .topic = {
        .name = "picoros/chatter",
        .type = ROSTYPE_NAME(ros_String),
        .rihs_hash = ROSTYPE_HASH(ros_String),
    },
};

static picoros_subscriber_t sub_listener = {
    .topic = {
        .name = "picoros/listener",
        .type = ROSTYPE_NAME(ros_String),
        .rihs_hash = ROSTYPE_HASH(ros_String),
    },
    .user_callback = listener_cb,
};

static void listener_cb(uint8_t *rx_data, size_t data_len)
{
    char *msg = NULL;
    ps_deserialize(rx_data, &msg, data_len);
    ESP_LOGI(TAG, "listener got: '%s'", msg ? msg : "(null)");
}

static uint8_t pub_buf[256];

void app_main(void)
{
    ethernet_start();

    picoros_interface_t ifx = {
        .mode = "client",
        .locator = CONFIG_PICOROS_ROUTER_LOCATOR,
    };

    ESP_LOGI(TAG, "connecting to rmw_zenohd at %s", ifx.locator);
    while (picoros_interface_init(&ifx) == PICOROS_NOT_READY) {
        ESP_LOGW(TAG, "router not reachable, retrying in 1 s");
        z_sleep_s(1);
    }

    ESP_LOGI(TAG, "node '%s' domain %u", node.name, (unsigned)node.domain_id);
    picoros_node_init(&node);
    picoros_publisher_declare(&node, &pub_chatter);
    picoros_subscriber_declare(&node, &sub_listener);
    ESP_LOGI(TAG, "publishing /%s, subscribed to /%s", pub_chatter.topic.name, sub_listener.topic.name);

    uint32_t count = 0;
    char text[64];
    while (true) {
        snprintf(text, sizeof(text), "Hello from ESP32-P4 Pico-ROS #%lu", (unsigned long)count++);
        char *msg = text;
        size_t len = ps_serialize(pub_buf, &msg, sizeof(pub_buf));
        picoros_res_t res = picoros_publish(&pub_chatter, pub_buf, len);
        ESP_LOGI(TAG, "pub #%lu %s (link %s)", (unsigned long)(count - 1),
                 res == PICOROS_OK ? "ok" : "FAILED", picoros_interface_is_up() ? "up" : "down");
        z_sleep_s(1);
    }
}
