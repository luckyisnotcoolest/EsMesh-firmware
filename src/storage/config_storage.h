#ifndef ESMESH_CONFIG_STORAGE_H
#define ESMESH_CONFIG_STORAGE_H

#include <esp_err.h>
#include <cJSON.h>

/* Configuration structure for persistent storage */
typedef struct {
    char device_name[65];           /* Max 64 chars + null */
    char wifi_ssid[33];             /* Max 32 chars + null */
    char wifi_password[65];         /* Max 64 chars + null */
    uint32_t wifi_use_static_ip;    /* 0 = DHCP, 1 = static */
    char wifi_static_ip[16];        /* e.g., "192.168.1.100" */
    char wifi_gateway[16];
    char wifi_subnet[16];
    char wifi_dns[16];
    uint32_t wifi_retry_count;
    uint32_t http_enabled;
    uint32_t websocket_enabled;
    uint32_t mdns_enabled;
} esmesh_config_t;

/**
 * Initialize configuration storage (NVS)
 * @return ESP_OK on success
 */
esp_err_t config_storage_init(void);

/**
 * Load configuration from NVS
 * Uses default values if configuration is missing/corrupted
 * @param config Output configuration structure
 * @return ESP_OK on success, ESP_ERR_NVS_NOT_FOUND if using defaults
 */
esp_err_t config_storage_load(esmesh_config_t* config);

/**
 * Save configuration to NVS
 * @param config Configuration structure to save
 * @return ESP_OK on success
 */
esp_err_t config_storage_save(const esmesh_config_t* config);

/**
 * Get configuration as JSON
 * Does NOT include passwords
 * @return cJSON* configuration object (must be freed by caller)
 */
cJSON* config_storage_to_json(const esmesh_config_t* config);

/**
 * Update configuration from JSON
 * Validates all fields
 * @param config Configuration to update
 * @param json JSON object with updates
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if validation fails
 */
esp_err_t config_storage_from_json(esmesh_config_t* config, cJSON* json);

/**
 * Reset configuration to defaults
 * @return ESP_OK on success
 */
esp_err_t config_storage_reset_defaults(void);

/**
 * Get default configuration
 * @param config Output structure
 * @return ESP_OK
 */
esp_err_t config_storage_get_defaults(esmesh_config_t* config);

#endif /* ESMESH_CONFIG_STORAGE_H */
