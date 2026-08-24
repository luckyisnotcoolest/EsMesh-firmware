#include "logging.h"
#include "validation.h"
#include "protocol.h"
#include <string.h>
#include <ctype.h>

int validate_device_name(const char* name) {
    if (!name || strlen(name) == 0 || strlen(name) > MAX_DEVICE_NAME_LEN) {
        return 0;
    }
    return 1;
}

int validate_ssid(const char* ssid) {
    if (!ssid || strlen(ssid) == 0 || strlen(ssid) > MAX_SSID_LEN) {
        return 0;
    }
    return 1;
}

int validate_password(const char* password) {
    if (!password || strlen(password) == 0 || strlen(password) > MAX_PASSWORD_LEN) {
        return 0;
    }
    return 1;
}

int validate_ip_address(const char* ip) {
    if (!ip || strlen(ip) == 0) {
        return 0;
    }

    int parts = 0;
    unsigned int octets[4] = {0};

    if (sscanf(ip, "%u.%u.%u.%u", &octets[0], &octets[1], &octets[2], &octets[3]) != 4) {
        return 0;
    }

    for (int i = 0; i < 4; i++) {
        if (octets[i] > 255) {
            return 0;
        }
    }

    return 1;
}

int validate_json_size(const char* json_str) {
    if (!json_str) {
        return 0;
    }
    return strlen(json_str) <= MAX_JSON_SIZE;
}

esp_err_t validate_json_fields(cJSON* json, const char** field_names, int field_count) {
    if (!json || !field_names || field_count <= 0) {
        return ESP_ERR_INVALID_ARG;
    }

    for (int i = 0; i < field_count; i++) {
        if (!cJSON_HasObjectItem(json, field_names[i])) {
            LOG_ERROR("Missing required field: %s", field_names[i]);
            return ESP_ERR_INVALID_ARG;
        }
    }

    return ESP_OK;
}
