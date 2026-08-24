#ifndef ESMESH_CAPABILITIES_H
#define ESMESH_CAPABILITIES_H

#include <cJSON.h>
#include <esp_err.h>

/**
 * Initialize capability detection
 * Detects actual hardware features
 * @return ESP_OK on success
 */
esp_err_t capabilities_init(void);

/**
 * Get capabilities as cJSON object
 * Only reports features actually implemented and available
 * @return cJSON* capabilities (must be freed by caller)
 */
cJSON* capabilities_get(void);

/**
 * Check if WiFi STA is available
 * @return 1 if available, 0 otherwise
 */
int capabilities_has_wifi_sta(void);

/**
 * Check if WiFi AP is available
 * @return 1 if available, 0 otherwise
 */
int capabilities_has_wifi_ap(void);

/**
 * Check if PSRAM is available
 * @return 1 if available, 0 otherwise
 */
int capabilities_has_psram(void);

/**
 * Check if 2.4 GHz WiFi is available
 * @return 1 if available, 0 otherwise
 */
int capabilities_has_wifi_2_4ghz(void);

/**
 * Get PSRAM size in bytes
 * @return Size in bytes, or 0 if not available
 */
uint32_t capabilities_get_psram_size(void);

/**
 * Get flash size in bytes
 * @return Size in bytes
 */
uint32_t capabilities_get_flash_size(void);

#endif /* ESMESH_CAPABILITIES_H */
