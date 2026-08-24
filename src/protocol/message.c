#include "protocol.h"
#include "logging.h"
#include "message.h"
#include "validation.h"
#include <cJSON.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <esp_random.h>

esp_err_t esmesh_message_from_json(const char* json_str, esmesh_message_t* msg) {
    if (!json_str || !msg) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!validate_json_size(json_str)) {
        LOG_ERROR("JSON too large");
        return ESP_ERR_INVALID_SIZE;
    }

    cJSON* root = cJSON_Parse(json_str);
    if (!root) {
        LOG_ERROR("Failed to parse JSON");
        return ESP_ERR_INVALID_ARG;
    }

    /* Validate protocol */
    cJSON* protocol_item = cJSON_GetObjectItem(root, "protocol");
    if (!protocol_item || !cJSON_IsString(protocol_item)) {
        LOG_ERROR("Missing or invalid protocol field");
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }
    if (strcmp(protocol_item->valuestring, ESMESH_PROTOCOL_VERSION) != 0) {
        LOG_ERROR("Protocol mismatch: expected %s, got %s", ESMESH_PROTOCOL_VERSION, protocol_item->valuestring);
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }

    /* Extract required fields */
    cJSON* id_item = cJSON_GetObjectItem(root, "id");
    if (!id_item || !cJSON_IsString(id_item) || !esmesh_is_valid_msg_id(id_item->valuestring)) {
        LOG_ERROR("Missing or invalid id");
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }
    strncpy(msg->id, id_item->valuestring, sizeof(msg->id) - 1);
    msg->id[sizeof(msg->id) - 1] = '\0';

    cJSON* source_item = cJSON_GetObjectItem(root, "source");
    if (!source_item || !cJSON_IsString(source_item) || !esmesh_is_valid_node_id(source_item->valuestring)) {
        LOG_ERROR("Missing or invalid source");
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }
    strncpy(msg->source, source_item->valuestring, sizeof(msg->source) - 1);
    msg->source[sizeof(msg->source) - 1] = '\0';

    cJSON* dest_item = cJSON_GetObjectItem(root, "destination");
    if (!dest_item || !cJSON_IsString(dest_item) || !esmesh_is_valid_node_id(dest_item->valuestring)) {
        LOG_ERROR("Missing or invalid destination");
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }
    strncpy(msg->destination, dest_item->valuestring, sizeof(msg->destination) - 1);
    msg->destination[sizeof(msg->destination) - 1] = '\0';

    cJSON* payload_item = cJSON_GetObjectItem(root, "payload");
    if (!payload_item || !cJSON_IsString(payload_item) || !esmesh_is_valid_payload(payload_item->valuestring)) {
        LOG_ERROR("Missing or invalid payload");
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }
    strncpy(msg->payload, payload_item->valuestring, sizeof(msg->payload) - 1);
    msg->payload[sizeof(msg->payload) - 1] = '\0';

    cJSON* timestamp_item = cJSON_GetObjectItem(root, "timestamp");
    if (!timestamp_item || !cJSON_IsNumber(timestamp_item)) {
        LOG_ERROR("Missing or invalid timestamp");
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }
    msg->timestamp = (uint32_t)timestamp_item->valuedouble;

    cJSON* ttl_item = cJSON_GetObjectItem(root, "ttl");
    if (!ttl_item || !cJSON_IsNumber(ttl_item) || ttl_item->valueint < 0 || ttl_item->valueint > MAX_TTL) {
        LOG_ERROR("Missing or invalid ttl");
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }
    msg->ttl = (uint8_t)ttl_item->valueint;

    cJSON_Delete(root);
    msg->received_at = (uint32_t)time(NULL);
    return ESP_OK;
}

char* esmesh_message_to_json(const esmesh_message_t* msg) {
    if (!msg) {
        return NULL;
    }

    cJSON* root = cJSON_CreateObject();
    if (!root) {
        return NULL;
    }

    cJSON_AddStringToObject(root, "protocol", ESMESH_PROTOCOL_VERSION);
    cJSON_AddStringToObject(root, "type", "message");
    cJSON_AddStringToObject(root, "id", msg->id);
    cJSON_AddStringToObject(root, "source", msg->source);
    cJSON_AddStringToObject(root, "destination", msg->destination);
    cJSON_AddNumberToObject(root, "timestamp", (double)msg->timestamp);
    cJSON_AddNumberToObject(root, "ttl", msg->ttl);
    cJSON_AddStringToObject(root, "payload", msg->payload);

    char* json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;
}

esp_err_t esmesh_generate_msg_id(char* id_out) {
    if (!id_out) {
        return ESP_ERR_INVALID_ARG;
    }

    uint32_t rand1 = esp_random();
    uint32_t rand2 = esp_random();
    snprintf(id_out, 33, "MSG-%08X%08X", rand1, rand2);
    return ESP_OK;
}

int esmesh_is_valid_msg_id(const char* id) {
    if (!id || strlen(id) == 0 || strlen(id) > MAX_MSG_ID_LEN) {
        return 0;
    }
    return 1;
}

int esmesh_is_valid_payload(const char* payload) {
    if (!payload || strlen(payload) == 0 || strlen(payload) > MAX_PAYLOAD_SIZE) {
        return 0;
    }
    return 1;
}

int esmesh_is_valid_node_id(const char* node_id) {
    if (!node_id || strlen(node_id) == 0 || strlen(node_id) > MAX_SOURCE_LEN) {
        return 0;
    }
    /* Basic validation: alphanumeric and hyphens */
    for (int i = 0; node_id[i]; i++) {
        char c = node_id[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
              (c >= '0' && c <= '9') || c == '-' || c == '_')) {
            return 0;
        }
    }
    return 1;
}
