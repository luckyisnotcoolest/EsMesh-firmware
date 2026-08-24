#ifndef ESMESH_JSON_BUILDER_H
#define ESMESH_JSON_BUILDER_H

#include <cJSON.h>
#include <esp_err.h>

/* JSON building utilities */

/**
 * Create a standard EsMesh API response wrapper
 * @param status The status or error message
 * @param payload Optional payload (can be NULL for simple responses)
 * @return cJSON* root object (must be freed by caller)
 */
cJSON* json_create_response(const char* status, cJSON* payload);

/**
 * Create a standard EsMesh error response
 * @param code Error code (e.g., "INVALID_JSON")
 * @param message Human-readable error message
 * @return cJSON* root object (must be freed by caller)
 */
cJSON* json_create_error(const char* code, const char* message);

/**
 * Create a device info object with actual hardware data
 * @return cJSON* device object (must be freed by caller)
 */
cJSON* json_create_device_info(void);

/**
 * Create a network info object with actual WiFi/IP data
 * @return cJSON* network object (must be freed by caller)
 */
cJSON* json_create_network_info(void);

/**
 * Create a capabilities object based on actual hardware
 * @return cJSON* capabilities object (must be freed by caller)
 */
cJSON* json_create_capabilities(void);

/**
 * Create a status object with current device state
 * @return cJSON* status object (must be freed by caller)
 */
cJSON* json_create_status(void);

/**
 * Convert cJSON to string with bounds checking
 * @param item The cJSON item
 * @param max_size Maximum allowed string size
 * @return char* allocated string (must be freed) or NULL
 */
char* json_to_string_safe(cJSON* item, size_t max_size);

#endif /* ESMESH_JSON_BUILDER_H */
