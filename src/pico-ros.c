/**
  ******************************************************************************
  * @file    pico-ros.c
  * @date    2025-May-27
  * @brief   Description
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "pico-ros.h"
#include "zenoh-pico/config.h"
#include "zenoh-pico/system/common/platform.h"
#include "zenoh-pico/utils/result.h"

#define _PR_LOG(...) printf(__VA_ARGS__)
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static z_owned_session_t s_wrapper;
static picoros_node_t* pnode;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void rmw_zenoh_gen_attachment_gid(rmw_attachment_t* attachment){
    attachment->rmw_gid_size = RMW_GID_SIZE;
    for (int i = 0; i < RMW_GID_SIZE; i++){
        attachment->rmw_gid[i] = z_random_u8();
    }
}

int rmw_zenoh_node_liveliness_keyexpr(const z_id_t *id, char *keyexpr){
    return snprintf(keyexpr, KEYEXPR_SIZE,
            "@ros2_lv/%d/%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x/0/0/NN/%%/%%/"
//            "%s_%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            "%s",
            pnode->domain_id,
            id->id[0], id->id[1],  id->id[2], id->id[3], id->id[4], id->id[5], id->id[6],
            id->id[7], id->id[8],  id->id[9], id->id[10], id->id[11], id->id[12], id->id[13],
            id->id[14], id->id[15],
            pnode->name
//            ,guid[0], guid[1], guid[2], guid[3],
//            guid[4], guid[5], guid[6], guid[7],
//            guid[8], guid[9], guid[10], guid[11],
//            guid[12], guid[13], guid[14], guid[15]
           );
}

int rmw_zenoh_topic_keyexpr(const char *topic, const char *rihs_hash, char *type, char *keyexpr){
    return snprintf(keyexpr, KEYEXPR_SIZE, "%u/%s/%s_/RIHS01_%s", pnode->domain_id, topic, type, rihs_hash );
}

int rmw_zenoh_topic_liveliness_keyexpr(const z_id_t *id, const char *topic, const char *rihs_hash,
        char *type, char *keyexpr, const char *entity_str) {
    char topic_lv[96];
    char *str = &topic_lv[0];

    strncpy(topic_lv, topic, 95);

    while (*str) {
        if (*str == '/') {
            *str = '%';
        }

        str++;
    }

   int ret = snprintf(keyexpr, KEYEXPR_SIZE,
            "@ros2_lv/%u/"
            "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x/"
//            "0/11/%s/%%/%%/mcb_%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x/%%%s/"
            "0/11/%s/%%/%%/%s/%%%s/"
            "%s_/RIHS01_%s"
            "/::,:,:,:,,",
            pnode->domain_id,
            id->id[0], id->id[1],  id->id[2], id->id[3], id->id[4], id->id[5], id->id[6],
            id->id[7], id->id[8],  id->id[9], id->id[10], id->id[11], id->id[12], id->id[13],
            id->id[14], id->id[15],
            entity_str, pnode->name,
//            guid[0], guid[1], guid[2], guid[3],
//            guid[4], guid[5], guid[6], guid[7],
//            guid[8], guid[9], guid[10], guid[11],
//            guid[12], guid[13], guid[14], guid[15],
            topic_lv, type, rihs_hash
               );

   return ret;
}

void _sub_data_handler(z_loaned_sample_t *sample, void *ctx) {
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

static void srv_data_handler(z_loaned_query_t *query, void *arg) {
    picoros_service_t* srv = (picoros_service_t*)arg;

    if (srv->user_callback == NULL){
        return;
    }

    const z_loaned_bytes_t *b = z_query_payload(query);
    size_t rx_data_len = _z_bytes_len(b);

    // get request data
    uint8_t* rx_data = (uint8_t*)z_malloc(rx_data_len);
    _z_bytes_to_buf(b, rx_data, rx_data_len);

    // process
    picoros_service_reply_t reply =
        srv->user_callback(rx_data, rx_data_len, srv->user_data);

    if (reply.data){
        // move reply to zbytes
        z_owned_bytes_t reply_payload;
        z_bytes_copy_from_buf(&reply_payload, reply.data, reply.length);

        // rmw attachment
        srv->attachment.sequence_number=1;
        srv->attachment.time = z_clock_now().tv_nsec;
        z_query_reply_options_t options;
        z_query_reply_options_default(&options);
        z_owned_bytes_t tx_attachment;
        z_bytes_from_static_buf(&tx_attachment, (uint8_t *)&srv->attachment, sizeof(rmw_attachment_t));
        options.attachment = z_bytes_move(&tx_attachment);

        // send reply
        z_query_reply(query, z_query_keyexpr(query), z_bytes_move(&reply_payload), &options);

        // cleanup
        z_bytes_drop(z_bytes_move(&reply_payload));
        if (reply.free_callback != NULL){
            reply.free_callback(reply.data);
        }
    }
    z_free(rx_data);
}

static void srv_drop_handler(void *arg) {

}

/* Public functions ----------------------------------------------------------*/

picoros_res_t picoros_init(picoros_node_t* node) {
	z_result_t res = Z_OK;
    pnode = node;
    // guid = node->guid;
	// node_name = node->name;

    z_owned_config_t config;
	z_config_default(&config);

	_PR_LOG("Configuring Zenoh session...\r\n");
    zp_config_insert(z_config_loan_mut(&config), Z_CONFIG_MODE_KEY, node->mode);
    if (node->locator) {
        if (strcmp(node->mode, "client") == 0) {
            zp_config_insert(z_config_loan_mut(&config), Z_CONFIG_CONNECT_KEY, node->locator);
        } else {
            zp_config_insert(z_config_loan_mut(&config), Z_CONFIG_LISTEN_KEY, node->locator);
        }
    }

	_PR_LOG("Opening Zenoh session...\r\n");
	if ((res = z_open(&s_wrapper, z_config_move(&config), NULL)) != Z_OK) {
	   _PR_LOG("Unable to open Zenoh session! Error:%d\n", res);
	   return PICOROS_NOT_READY;
	}
	_PR_LOG("Zenoh setup finished!\r\n");

	// Start read and lease tasks for zenoh-pico
	if ((res = zp_start_read_task(z_session_loan_mut(&s_wrapper), NULL)) != Z_OK
	|| (res = zp_start_lease_task(z_session_loan_mut(&s_wrapper), NULL)) != Z_OK
	){
	   z_session_drop(z_session_move(&s_wrapper));
	   _PR_LOG("Failed to start read/lease tasks! Error:%d\n", res);
	   return PICOROS_ERROR;
	}

	z_id_t self_id = z_info_zid(z_session_loan(&s_wrapper));
	char keyexpr[KEYEXPR_SIZE];

	rmw_zenoh_node_liveliness_keyexpr(&self_id, keyexpr);
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

picoros_res_t picoros_publisher_declare(picoros_publisher_t *pub) {
    z_view_keyexpr_t ke;
    z_result_t res = Z_OK;
    char keyexpr[KEYEXPR_SIZE];
    if (pub->topic.type != NULL){
        rmw_zenoh_topic_keyexpr(pub->topic.name, pub->topic.rihs_hash, pub->topic.type, keyexpr);
        z_view_keyexpr_from_str_unchecked(&ke, keyexpr);
    }
    else{
        z_view_keyexpr_from_str_unchecked(&ke, pub->topic.name);
    }
    rmw_zenoh_gen_attachment_gid(&pub->attachment);

    if ((res = z_declare_publisher(z_session_loan(&s_wrapper), &pub->zpub, z_view_keyexpr_loan(&ke), NULL)) != Z_OK){
        _PR_LOG("Unable to declare node liveliness token! Error:%d\n", res);
        return PICOROS_ERROR;
    }

    if (pub->topic.type != NULL){
        z_view_keyexpr_t ke2;
        z_id_t zid = z_info_zid(z_session_loan(&s_wrapper));
        rmw_zenoh_topic_liveliness_keyexpr(&zid, pub->topic.name, pub->topic.rihs_hash, pub->topic.type, keyexpr, "MP");
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
picoros_res_t picoros_publish(picoros_publisher_t *pub, uint8_t *payload, size_t len) {
    z_result_t res = Z_OK;
    z_publisher_put_options_t options;
    z_publisher_put_options_default(&options);

//    int64_t time_ns = tx_time_get()*1000000000ULL/TX_TIMER_TICKS_PER_SECOND;
    pub->attachment.sequence_number++;
    pub->attachment.time = z_clock_now().tv_nsec;

    z_owned_bytes_t z_attachment;
    z_bytes_from_static_buf(&z_attachment, (uint8_t *)&pub->attachment, sizeof(rmw_attachment_t));

    options.attachment = z_bytes_move(&z_attachment);

    z_owned_bytes_t zbytes;
    z_bytes_from_static_buf(&zbytes, payload, len);

    if ((res = z_publisher_put(z_publisher_loan(&pub->zpub), z_bytes_move(&zbytes), &options)) != Z_OK){
        _PR_LOG("Unable to publish payload! Error:%d\n", res);
        return PICOROS_ERROR;
    }
    return PICOROS_OK;
}

// Subscribe to a topic
picoros_res_t picoros_subscriber_declare(picoros_subscriber_t *sub) {
	char keyexpr[KEYEXPR_SIZE];
	z_view_keyexpr_t ke;
    z_result_t res = Z_OK;
	if(sub->topic.type != NULL){
        rmw_zenoh_topic_keyexpr(sub->topic.name, sub->topic.rihs_hash, sub->topic.type, keyexpr);
        z_view_keyexpr_from_str_unchecked(&ke, keyexpr);
	}
	else{
        z_view_keyexpr_from_str_unchecked(&ke, sub->topic.name);
	}

	z_owned_closure_sample_t callback;
    z_closure_sample(&callback, _sub_data_handler, NULL, NULL);
	callback._val.context = sub->user_callback;

	if ((res = z_declare_subscriber(z_session_loan(&s_wrapper), &sub->zsub, z_view_keyexpr_loan(&ke), z_closure_sample_move(&callback), NULL)) != Z_OK) {
        _PR_LOG("Unable to declare subscriber! Error:%d\n", res);
        return PICOROS_ERROR;
	}

	if(sub->topic.type != NULL){
        z_id_t zid = z_info_zid(z_session_loan(&s_wrapper));
        rmw_zenoh_topic_liveliness_keyexpr(&zid, sub->topic.name, sub->topic.rihs_hash, sub->topic.type, keyexpr, "MS");

        z_view_keyexpr_from_str(&ke, keyexpr);

        z_owned_liveliness_token_t token;

        if ((res = z_liveliness_declare_token(z_session_loan(&s_wrapper), &token, z_view_keyexpr_loan(&ke), NULL)) != Z_OK) {
            _PR_LOG("Unable to declare subscriber liveliness token! Error:%d\n", res);
            return PICOROS_ERROR;
        }
	}
    return PICOROS_OK;

}

#if 0
z_result_t zenoh_query(
        char* key,
        z_get_options_t* opts,
        _z_closure_reply_callback_t reply_cb,
        z_closure_drop_callback_t drop_cb,
        uint8_t* data, size_t size
){
    z_result_t res;
    z_owned_closure_reply_t callback;
    z_view_keyexpr_t ke;
    z_view_keyexpr_from_str_unchecked(&ke, key);
    // Value encoding
    z_owned_bytes_t payload;
    z_closure_reply(&callback, reply_cb, drop_cb, NULL);
    if (data != NULL && size > 0){
        z_bytes_from_static_buf(&payload, data, size);
        opts->payload = z_bytes_move(&payload);
    }
    res = z_get(z_session_loan(&s_wrapper), z_view_keyexpr_loan(&ke), "", z_closure_reply_move(&callback), opts);
    z_bytes_drop(z_bytes_move(&payload));
    z_bytes_drop(opts->payload);
    z_closure_reply_drop(z_closure_reply_move(&callback));
    return res;
}
#endif

picoros_res_t picoros_service_declare(picoros_service_t* srv){
   z_result_t res;
   char keyexpr[KEYEXPR_SIZE];

   z_view_keyexpr_t ke;
   if (srv->topic.type != NULL){
       rmw_zenoh_topic_keyexpr(srv->topic.name, srv->topic.rihs_hash, srv->topic.type, keyexpr);
       z_view_keyexpr_from_str_unchecked(&ke, keyexpr);
   }
   else{
       z_view_keyexpr_from_str_unchecked(&ke, srv->topic.name);
   }

   rmw_zenoh_gen_attachment_gid(&srv->attachment);

   z_queryable_options_t options = {};
   options.complete = true;   //needed for rmw_zenoh

   z_owned_closure_query_t callback;
   z_closure_query(&callback, srv_data_handler, srv_drop_handler, srv);
   if((res = z_declare_queryable(z_session_loan(&s_wrapper), &srv->zqable, z_view_keyexpr_loan(&ke), z_closure_query_move(&callback), &options)) != Z_OK){
        _PR_LOG("Unable to declare service! Error:%d\n", res);
        return PICOROS_ERROR;
   }
   if (srv->topic.type != NULL){
       z_view_keyexpr_t ke2;
       z_owned_liveliness_token_t token;
       z_id_t zid = z_info_zid(z_session_loan(&s_wrapper));
       rmw_zenoh_topic_liveliness_keyexpr(&zid, srv->topic.name, srv->topic.rihs_hash, srv->topic.type, keyexpr, "SS");
       z_view_keyexpr_from_str(&ke2, keyexpr);

       if ((res = z_liveliness_declare_token(z_session_loan(&s_wrapper), &token, z_view_keyexpr_loan(&ke2), NULL)) != Z_OK) {
         _PR_LOG("Unable to declare service liveliness token! Error:%d\n", res);
         return PICOROS_ERROR;
       }
   }
   return PICOROS_OK;
}



picoros_res_t picoros_unsubscribe(picoros_subscriber_t *sub) {
	return (z_undeclare_subscriber(z_subscriber_move(&sub->zsub)) == Z_OK) ? PICOROS_OK : PICOROS_ERROR;
}
