/*******************************************************************************
 * @file    picoros.c
 * @brief   Pico-ROS Core Implementation
 * @date    2025-May-27
 *
 * @details This file implements the core functionality of Pico-ROS, including
 *          node management, publisher/subscriber communication, and service
 *          server implementation.
 *
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include <string.h>
#include <inttypes.h>
#include "picoros.h"

#ifdef PICOROS_DEBUG
    #include <stdio.h>
    #define _PR_LOG(...) printf(__VA_ARGS__)
#else
    #define _PR_LOG(...)
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static z_owned_session_t s_wrapper;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void rmw_zenoh_gen_attachment_gid(rmw_attachment_t* attachment) {
    attachment->rmw_gid_size = RMW_GID_SIZE;
    for (int i = 0; i < RMW_GID_SIZE; i++) {
        attachment->rmw_gid[i] = z_random_u8();
    }
}

static int rmw_zenoh_node_liveliness_keyexpr(picoros_node_t* node, char* keyexpr) {
#if USE_NODE_GUID == 1
    uint8_t* guid = node->guid;
#endif
    z_id_t id = z_info_zid(z_session_loan(&s_wrapper));
    return snprintf(keyexpr, KEYEXPR_SIZE,
            "@ros2_lv/%" PRIu32 "/%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x/0/0/NN/%%/%%/"
#if USE_NODE_GUID == 1
            "%s_%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
#else
            "%s",
#endif
            node->domain_id,
            id.id[0], id.id[1],  id.id[2], id.id[3], id.id[4], id.id[5], id.id[6],
            id.id[7], id.id[8],  id.id[9], id.id[10], id.id[11], id.id[12], id.id[13],
            id.id[14], id.id[15],
#if USE_NODE_GUID == 1
            node->name, guid[0], guid[1], guid[2], guid[3],
            guid[4], guid[5], guid[6], guid[7],
            guid[8], guid[9], guid[10], guid[11],
            guid[12], guid[13], guid[14], guid[15]
#else
            node->name
#endif
           );
}

static int rmw_zenoh_topic_keyexpr(picoros_node_t* node, rmw_topic_t* topic, char* keyexpr) {
    char topic_lv[96];
    char *str = &topic_lv[0];
    strncpy(topic_lv, topic->name, 95);
    // replace / with %
    while (*str) {
        if (*str == '/') {
            *str = '%';
        }
        str++;
    }
    return snprintf(keyexpr, KEYEXPR_SIZE, "%" PRIu32 "/%s/%s_/RIHS01_%s", node->domain_id, topic_lv, topic->type,
                    topic->rihs_hash);
}

static int rmw_zenoh_service_keyexpr(picoros_node_t* node, rmw_topic_t* topic, char* keyexpr) {
    char topic_lv[96];
    char *str = &topic_lv[0];
    strncpy(topic_lv, topic->name, 95);
    // replace / with %
    while (*str) {
        if (*str == '/') {
            *str = '%';
        }
        str++;
    }
    if (node->name == NULL){
        return snprintf(keyexpr, KEYEXPR_SIZE, "%" PRIu32 "/%s/%s_/RIHS01_%s", node->domain_id, topic_lv,
                            topic->type, topic->rihs_hash);
    }
    else{
        return snprintf(keyexpr, KEYEXPR_SIZE, "%" PRIu32 "/%s%%%s/%s_/RIHS01_%s", node->domain_id, node->name, topic_lv,
                            topic->type, topic->rihs_hash);
    }
}

static int rmw_zenoh_topic_liveliness_keyexpr(picoros_node_t* node, rmw_topic_t* topic, char *keyexpr, const char *entity_str) {
#if USE_NODE_GUID == 1
    uint8_t* guid = node->guid;
#endif
    char topic_lv[96];
    char *str = &topic_lv[0];

    z_id_t id = z_info_zid(z_session_loan(&s_wrapper));

    if (strcmp(entity_str, "SS") == 0){
        // is service
        snprintf(topic_lv, 95, "%s/%s", node->name, topic->name);
    }
    else{
        strncpy(topic_lv, topic->name, 95);
    }

    // replace / with %
    while (*str) {
        if (*str == '/') {
            *str = '%';
        }
        str++;
    }

    int ret = snprintf(keyexpr, KEYEXPR_SIZE,
            "@ros2_lv/%" PRIu32 "/"
            "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x/"
#if USE_NODE_GUID == 1
           "0/11/%s/%%/%%/%s_%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x/%%%s/"
#else
            "0/11/%s/%%/%%/%s/%%%s/"
#endif
            "%s_/RIHS01_%s"
            "/::,:,:,:,,",
            node->domain_id,
            id.id[0], id.id[1],  id.id[2], id.id[3], id.id[4], id.id[5], id.id[6],
            id.id[7], id.id[8],  id.id[9], id.id[10], id.id[11], id.id[12], id.id[13],
            id.id[14], id.id[15],
            entity_str, node->name,
#if USE_NODE_GUID == 1
            guid[0], guid[1], guid[2], guid[3],
            guid[4], guid[5], guid[6], guid[7],
            guid[8], guid[9], guid[10], guid[11],
            guid[12], guid[13], guid[14], guid[15],
#endif
            topic_lv, topic->type, topic->rihs_hash
               );

   return ret;
}

static void sub_data_handler(z_loaned_sample_t *sample, void *ctx) {
    const z_loaned_bytes_t *b = z_sample_payload(sample);

    size_t raw_data_len = _z_bytes_len(b);
    if (raw_data_len == 0) {
        return;
    }
    uint8_t *raw_data = (uint8_t*)z_malloc(raw_data_len);
    _z_bytes_to_buf(b, raw_data, raw_data_len);

    // Call user callback function if given:
    if (ctx != NULL) {
    	((picoros_sub_cb_t)ctx)(raw_data, raw_data_len);
    }
    z_free(raw_data);
}

static void queriable_data_handler(z_loaned_query_t *query, void *arg) {
    picoros_srv_server_t* srv = (picoros_srv_server_t*)arg;

    if (srv->user_callback == NULL){
        return;
    }

    const z_loaned_bytes_t *b = z_query_payload(query);
    size_t rx_data_len = _z_bytes_len(b);

    // get request data
    uint8_t* rx_data = (uint8_t*)z_malloc(rx_data_len);
    _z_bytes_to_buf(b, rx_data, rx_data_len);

    // process
    picoros_service_reply_t reply = srv->user_callback(srv, rx_data, rx_data_len);

    if (reply.data) {
        // move reply to zbytes
        z_owned_bytes_t reply_payload;
        z_bytes_copy_from_buf(&reply_payload, reply.data, reply.length);

        // rmw attachment
        srv->attachment.sequence_number = 1;
        srv->attachment.time = z_clock_now().tv_nsec;
        z_query_reply_options_t options;
        z_query_reply_options_default(&options);
        z_owned_bytes_t tx_attachment;
        z_bytes_from_static_buf(&tx_attachment, (uint8_t*)&srv->attachment, sizeof(rmw_attachment_t));
        options.attachment = z_bytes_move(&tx_attachment);

        // send reply
        z_result_t res = z_query_reply(query, z_query_keyexpr(query), z_bytes_move(&reply_payload), &options);
        if (res != Z_OK) {
            _PR_LOG("Error sending service reply. Error:%d\n", res);
        }

        // cleanup
        z_bytes_drop(z_bytes_move(&reply_payload));
        if (reply.free_callback != NULL) {
            reply.free_callback(reply.data);
        }
    }
    z_free(rx_data);
}

static void queriable_drop_handler(void* arg) { _PR_LOG("Drop srv callback\n"); }

static void get_drop_handler(void* ctx){
    picoros_srv_client_t* client = (picoros_srv_client_t*)ctx;
    client->_in_progress = false;
    if(client->drop_callback != NULL){
        client->drop_callback(client);
    }
}

static void get_data_handler(z_loaned_reply_t *reply, void *ctx){
    if (ctx == NULL){
        return;
    }
    size_t raw_data_len = 0;
    uint8_t* raw_data  = 0;
    bool error = false;
    const z_loaned_sample_t* sample = 0;
    const z_loaned_bytes_t* payload = 0;
    const z_loaned_reply_err_t* err = 0;

    if (z_reply_is_ok(reply)) {
        sample = z_reply_ok(reply);
        payload = z_sample_payload(sample);
    }
    else {
        err = z_reply_err(reply);
        payload = z_reply_err_payload(err);
        error = true;
    }

    raw_data_len = _z_bytes_len(payload);
    if (raw_data_len == 0) {
        return;
    }
    raw_data = (uint8_t*)z_malloc(raw_data_len);
    _z_bytes_to_buf(payload, raw_data, raw_data_len);

    picoros_srv_client_t* client = (picoros_srv_client_t*)ctx;
    client->user_callback(client, raw_data, raw_data_len, error);
    z_free(raw_data);
}

/* Public functions ----------------------------------------------------------*/

picoros_res_t picoros_interface_init(picoros_interface_t* ifx) {
    z_result_t res = Z_OK;
    z_owned_config_t config;
    z_config_default(&config);

    _PR_LOG("Configuring Zenoh session...\r\n");
    zp_config_insert(z_config_loan_mut(&config), Z_CONFIG_MODE_KEY, ifx->mode);
    if (ifx->locator) {
        if (strcmp(ifx->mode, "client") == 0) {
            zp_config_insert(z_config_loan_mut(&config), Z_CONFIG_CONNECT_KEY, ifx->locator);
        }
        else {
            zp_config_insert(z_config_loan_mut(&config), Z_CONFIG_LISTEN_KEY, ifx->locator);
        }
    }

    _PR_LOG("Opening Zenoh session...\r\n");
    if ((res = z_open(&s_wrapper, z_config_move(&config), NULL)) != Z_OK) {
        _PR_LOG("Unable to open Zenoh session! Error:%d\n", res);
        return PICOROS_NOT_READY;
    }
    _PR_LOG("Zenoh setup finished!\r\n");

    // Start read and lease tasks for zenoh-pico
    if((res = zp_start_read_task(z_session_loan_mut(&s_wrapper), NULL)) != Z_OK
    || (res = zp_start_lease_task(z_session_loan_mut(&s_wrapper), NULL)) != Z_OK
    ){
        z_session_drop(z_session_move(&s_wrapper));
        _PR_LOG("Failed to start read/lease tasks! Error:%d\n", res);
        return PICOROS_ERROR;
    }

    return PICOROS_OK;
}

picoros_res_t picoros_node_init(picoros_node_t* node) {
    z_result_t res = Z_OK;
    char keyexpr[KEYEXPR_SIZE];

    rmw_zenoh_node_liveliness_keyexpr(node, keyexpr);
    z_view_keyexpr_t ke;

    z_view_keyexpr_from_str(&ke, keyexpr);

    z_owned_liveliness_token_t token;

    if ((res = z_liveliness_declare_token(z_session_loan(&s_wrapper), &token, z_view_keyexpr_loan(&ke), NULL)) != Z_OK) {
        _PR_LOG("Unable to declare node liveliness token! Error:%d\n", res);
        return PICOROS_ERROR;
    }
    return PICOROS_OK;
}

void zenoh_shutdown() {
    z_session_drop(z_session_move(&s_wrapper));
}

picoros_res_t picoros_publisher_declare(picoros_node_t* node, picoros_publisher_t* pub) {
    z_view_keyexpr_t ke;
    z_result_t res = Z_OK;
    char keyexpr[KEYEXPR_SIZE];
    if (pub->topic.type != NULL) {
        rmw_zenoh_topic_keyexpr(node, &pub->topic, keyexpr);
        z_view_keyexpr_from_str_unchecked(&ke, keyexpr);
    }
    else {
        z_view_keyexpr_from_str_unchecked(&ke, pub->topic.name);
    }

    rmw_zenoh_gen_attachment_gid(&pub->attachment);

    if ((res = z_declare_publisher(z_session_loan(&s_wrapper), &pub->zpub, z_view_keyexpr_loan(&ke), NULL)) != Z_OK) {
        _PR_LOG("Unable to declare node liveliness token! Error:%d\n", res);
        return PICOROS_ERROR;
    }

    if (pub->topic.type != NULL) {
        z_view_keyexpr_t ke2;
        rmw_zenoh_topic_liveliness_keyexpr(node, &pub->topic, keyexpr, "MP");
        z_view_keyexpr_from_str(&ke2, keyexpr);

        z_owned_liveliness_token_t token;
        if ((res = z_liveliness_declare_token(z_session_loan(&s_wrapper), &token, z_view_keyexpr_loan(&ke2), NULL)) != Z_OK) {
            _PR_LOG("Unable to declare publisher liveliness token! Error:%d\n", res);
            return PICOROS_ERROR;
        }
    }
    return PICOROS_OK;
}

// Publish to a topic
picoros_res_t picoros_publish(picoros_publisher_t* pub, uint8_t* payload, size_t len) {
    z_result_t res = Z_OK;
    z_publisher_put_options_t options;
    z_publisher_put_options_default(&options);

    pub->attachment.sequence_number++;
    pub->attachment.time = z_clock_now().tv_nsec;

    z_owned_bytes_t z_attachment;
    z_bytes_from_static_buf(&z_attachment, (uint8_t*)&pub->attachment, sizeof(rmw_attachment_t));

    options.attachment = z_bytes_move(&z_attachment);

    z_owned_bytes_t zbytes;
    z_bytes_from_static_buf(&zbytes, payload, len);

    if ((res = z_publisher_put(z_publisher_loan(&pub->zpub), z_bytes_move(&zbytes), &options)) != Z_OK) {
        _PR_LOG("Unable to publish payload! Error:%d\n", res);
        return PICOROS_ERROR;
    }
    return PICOROS_OK;
}

// Subscribe to a topic
picoros_res_t picoros_subscriber_declare(picoros_node_t* node, picoros_subscriber_t* sub) {
    char keyexpr[KEYEXPR_SIZE];
    z_view_keyexpr_t ke;
    z_result_t res = Z_OK;
    if (sub->topic.type != NULL) {
        rmw_zenoh_topic_keyexpr(node, &sub->topic, keyexpr);
        z_view_keyexpr_from_str_unchecked(&ke, keyexpr);
    }
    else {
        z_view_keyexpr_from_str_unchecked(&ke, sub->topic.name);
    }

    z_owned_closure_sample_t callback;
    z_closure_sample(&callback, sub_data_handler, NULL, NULL);
    callback._val.context = sub->user_callback;

    if ((res = z_declare_subscriber(z_session_loan(&s_wrapper), &sub->zsub, z_view_keyexpr_loan(&ke),
                                    z_closure_sample_move(&callback), NULL)) != Z_OK) {
        _PR_LOG("Unable to declare subscriber! Error:%d\n", res);
        return PICOROS_ERROR;
    }

    if (sub->topic.type != NULL) {
        rmw_zenoh_topic_liveliness_keyexpr(node, &sub->topic, keyexpr, "MS");
        z_view_keyexpr_from_str(&ke, keyexpr);
        z_owned_liveliness_token_t token;
        if ((res = z_liveliness_declare_token(z_session_loan(&s_wrapper), &token, z_view_keyexpr_loan(&ke), NULL)) != Z_OK) {
            _PR_LOG("Unable to declare subscriber liveliness token! Error:%d\n", res);
            return PICOROS_ERROR;
        }
    }
    return PICOROS_OK;
}

picoros_res_t picoros_service_declare(picoros_node_t* node, picoros_srv_server_t* srv) {
    z_result_t res;
    char keyexpr[KEYEXPR_SIZE];

    z_view_keyexpr_t ke;
    if (srv->topic.type != NULL) {
        rmw_zenoh_service_keyexpr(node, &srv->topic, keyexpr);
        z_view_keyexpr_from_str_unchecked(&ke, keyexpr);
    }
    else {
        z_view_keyexpr_from_str_unchecked(&ke, srv->topic.name);
    }

    rmw_zenoh_gen_attachment_gid(&srv->attachment);

    z_queryable_options_t options = {};
    options.complete = true; // needed for rmw_zenoh

    z_owned_closure_query_t callback;
    z_closure_query(&callback, queriable_data_handler, queriable_drop_handler, srv);
    if ((res = z_declare_queryable(z_session_loan(&s_wrapper), &srv->zqable, z_view_keyexpr_loan(&ke),
                                   z_closure_query_move(&callback), &options)) != Z_OK) {
        _PR_LOG("Unable to declare service! Error:%d\n", res);
        return PICOROS_ERROR;
    }
    if (srv->topic.type != NULL) {
        z_view_keyexpr_t ke2;
        z_owned_liveliness_token_t token;
        rmw_zenoh_topic_liveliness_keyexpr(node, &srv->topic, keyexpr, "SS");
        z_view_keyexpr_from_str(&ke2, keyexpr);
        if ((res = z_liveliness_declare_token(z_session_loan(&s_wrapper), &token, z_view_keyexpr_loan(&ke2), NULL)) != Z_OK) {
            _PR_LOG("Unable to declare service liveliness token! Error:%d\n", res);
            return PICOROS_ERROR;
        }
    }
    return PICOROS_OK;
}


picoros_res_t picoros_service_client_init(picoros_srv_client_t * client){
    if (client->_key_buf == NULL){
        client->_key_buf = z_malloc(KEYEXPR_SIZE);
    }
    // Generate key expressions
    if (client->topic.type != NULL) {
        picoros_node_t node = {
            .domain_id = client->node_domain_id,
            .name = client->node_name,
        };
        rmw_zenoh_service_keyexpr(&node, &client->topic, client->_key_buf);
        z_view_keyexpr_from_str_unchecked(&client->ke, client->_key_buf);
    }
    else {
        z_view_keyexpr_from_str_unchecked(&client->ke, client->topic.name);
    }
    return PICOROS_OK;
}


picoros_res_t picoros_service_call(picoros_srv_client_t * client, uint8_t* payload, size_t len){
    if (client == NULL) { return PICOROS_ERROR;}
    if (client->_in_progress) { return PICOROS_NOT_READY;}

    z_result_t res;

    // create key expression if not done before
    if (client->_key_buf == NULL){
        picoros_service_client_init(client);
    }

    // Default options
    z_get_options_t default_opts;
    z_get_options_default(&default_opts);
    z_get_options_t* opts = &default_opts;
    if (client->opts != NULL){
        opts = client->opts;
    }

    // Payload
    z_owned_bytes_t zbytes;
    z_bytes_copy_from_buf(&zbytes, payload, len);
    opts->payload = z_bytes_move(&zbytes);

    // RMW attachment
    rmw_attachment_t attachment = {
        .rmw_gid_size = RMW_GID_SIZE,
        .sequence_number = 1,
        .time = z_clock_now().tv_nsec,
    };
    z_owned_bytes_t tx_attachment;
    z_bytes_copy_from_buf(&tx_attachment, (uint8_t*)&attachment, sizeof(rmw_attachment_t));
    opts->attachment = z_bytes_move(&tx_attachment);

    // Closure
    z_owned_closure_reply_t callback = {
        ._val.call = get_data_handler,
        ._val.drop = get_drop_handler,
        ._val.context = client,
    };

    client->_in_progress = true;
    if ((res = z_get(z_session_loan(&s_wrapper), z_view_keyexpr_loan(&client->ke), "", z_closure_reply_move(&callback), opts)) != Z_OK) {
        _PR_LOG("Error calling %s service! Error:%d\n", client->topic.name, res);
        client->_in_progress = false;
        z_bytes_drop(opts->attachment);
        z_bytes_drop(opts->payload);
        return PICOROS_ERROR;
    }
    return PICOROS_OK;
}

bool picoros_service_call_in_progress(picoros_srv_client_t* client){
    return client->_in_progress;
}

picoros_res_t picoros_unsubscribe(picoros_subscriber_t* sub) {
    return (z_undeclare_subscriber(z_subscriber_move(&sub->zsub)) == Z_OK) ? PICOROS_OK : PICOROS_ERROR;
}
