#include "device_id.h"
#include "logging.h"
#include "../protocol/protocol.h"
#include <esp_err.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_random.h>
#include <string.h>
#include <stdio.h>

#define DEVICE_ID_NVS_KEY "device_id"
#define DEVICE_ID_NVS_NAMESPACE "esmesh"

static char cached_device_id[DEVICE_ID_LEN + 1] = {0};

esp_err_t device_id_init(void) {
    LOG_BOOT("Initializing device ID...");

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(DEVICE_ID_NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        LOG_ERROR("Failed to open NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    /* Try to read existing device ID */
    size_t required_size = DEVICE_ID_LEN + 1;
    char device_id[DEVICE_ID_LEN + 1] = {0};
    err = nvs_get_str(nvs_handle, DEVICE_ID_NVS_KEY, device_id, &required_size);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        /* Generate new device ID */
        LOG_BOOT("Device ID not found, generating new one...");

        uint32_t rand_val = esp_random() & 0xFFFFFF; /* 24-bit random */
        snprintf(device_id, sizeof(device_id), "ESM-%06X", rand_val);

        err = nvs_set_str(nvs_handle, DEVICE_ID_NVS_KEY, device_id);
        if (err != ESP_OK) {
            LOG_ERROR("Failed to save device ID: %s", esp_err_to_name(err));
            nvs_close(nvs_handle);
            return err;
        }

        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            LOG_ERROR("Failed to commit NVS: %s", esp_err_to_name(err));
            nvs_close(nvs_handle);
            return err;
        }

        LOG_BOOT("Device ID generated: %s", device_id);
    } else if (err != ESP_OK) {
        LOG_ERROR("Failed to read device ID from NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    } else {
        LOG_BOOT("Device ID loaded: %s", device_id);
    }

    strncpy(cached_device_id, device_id, sizeof(cached_device_id) - 1);
    cached_device_id[sizeof(cached_device_id) - 1] = '\0';

    nvs_close(nvs_handle);
    return ESP_OK;
}

esp_err_t device_id_get(char* id_out) {
    if (!id_out) {
        return ESP_ERR_INVALID_ARG;
    }

    if (cached_device_id[0] == '\0') {
        return ESP_ERR_INVALID_STATE;
    }

    strncpy(id_out, cached_device_id, DEVICE_ID_LEN);
    id_out[DEVICE_ID_LEN] = '\0';
    return ESP_OK;
}

esp_err_t device_id_regenerate(void) {
    LOG_BOOT("Regenerating device ID (factory reset)...");

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(DEVICE_ID_NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        LOG_ERROR("Failed to open NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    uint32_t rand_val = esp_random() & 0xFFFFFF;
    char device_id[DEVICE_ID_LEN + 1];
    snprintf(device_id, sizeof(device_id), "ESM-%06X", rand_val);

    err = nvs_set_str(nvs_handle, DEVICE_ID_NVS_KEY, device_id);
    if (err != ESP_OK) {
        LOG_ERROR("Failed to save device ID: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    err = nvs_commit(nvs_handle);
    nvs_close(nvs_handle);

    if (err == ESP_OK) {
        strncpy(cached_device_id, device_id, sizeof(cached_device_id) - 1);
        cached_device_id[sizeof(cached_device_id) - 1] = '\0';
        LOG_BOOT("Device ID regenerated: %s", device_id);
    }

    return err;
}
