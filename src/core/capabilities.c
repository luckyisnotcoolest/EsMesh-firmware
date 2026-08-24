#include "capabilities.h"
#include "logging.h"
#include "../protocol/protocol.h"
#include <esp_chip_info.h>
#include <esp_psram.h>
#include <esp_flash.h>
#include <cJSON.h>

esp_err_t capabilities_init(void) {
    LOG_BOOT("Detecting device capabilities...");
    return ESP_OK;
}

cJSON* capabilities_get(void) {
    cJSON* cap = cJSON_CreateObject();
    if (!cap) {
        return NULL;
    }

    /* WiFi capabilities */
    cJSON_AddBoolToObject(cap, "wifi_sta", 1);
    cJSON_AddBoolToObject(cap, "wifi_ap", 0);  /* Not yet implemented */
    cJSON_AddBoolToObject(cap, "wifi_2_4ghz", 1);

    /* Network capabilities */
    cJSON_AddBoolToObject(cap, "http", 1);
    cJSON_AddBoolToObject(cap, "websocket", 1);
    cJSON_AddBoolToObject(cap, "mdns", 1);

    /* Features */
    cJSON_AddBoolToObject(cap, "mesh", 0);  /* Single node only for now */
    cJSON_AddBoolToObject(cap, "repeater", 0);
    cJSON_AddBoolToObject(cap, "bluetooth", 0);  /* Not yet implemented */
    cJSON_AddBoolToObject(cap, "ota_update", 0);  /* Not yet implemented */

    /* Hardware */
    cJSON_AddBoolToObject(cap, "psram", capabilities_has_psram());

    return cap;
}

int capabilities_has_wifi_sta(void) {
    return 1;  /* All ESP32-S3 have WiFi STA */
}

int capabilities_has_wifi_ap(void) {
    return 0;  /* Not yet implemented */
}

int capabilities_has_psram(void) {
    return esp_psram_is_initialized();
}

int capabilities_has_wifi_2_4ghz(void) {
    return 1;  /* All ESP32-S3 support 2.4 GHz */
}

uint32_t capabilities_get_psram_size(void) {
    if (!capabilities_has_psram()) {
        return 0;
    }
    return esp_psram_get_size();
}

uint32_t capabilities_get_flash_size(void) {
    return esp_flash_get_size(NULL);
}
