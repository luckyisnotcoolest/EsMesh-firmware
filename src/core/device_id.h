#ifndef ESMESH_DEVICE_ID_H
#define ESMESH_DEVICE_ID_H

#include <esp_err.h>

#define DEVICE_ID_LEN 10  /* ESM-XXXXXX = 10 chars */

/**
 * Initialize device ID system
 * Generates and stores device ID if not present
 * @return ESP_OK on success
 */
esp_err_t device_id_init(void);

/**
 * Get the device ID (e.g., "ESM-A1B2C3")
 * @param id_out Buffer to store ID (min 11 bytes for null terminator)
 * @return ESP_OK on success
 */
esp_err_t device_id_get(char* id_out);

/**
 * Regenerate the device ID (for factory reset)
 * @return ESP_OK on success
 */
esp_err_t device_id_regenerate(void);

#endif /* ESMESH_DEVICE_ID_H */
