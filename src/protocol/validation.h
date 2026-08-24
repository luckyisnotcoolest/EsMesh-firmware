#ifndef ESMESH_VALIDATION_H
#define ESMESH_VALIDATION_H

#include <cJSON.h>
#include <esp_err.h>

/**
 * Validate device name format
 * @param name Device name
 * @return 1 if valid, 0 otherwise
 */
int validate_device_name(const char* name);

/**
 * Validate WiFi SSID
 * @param ssid WiFi SSID
 * @return 1 if valid, 0 otherwise
 */
int validate_ssid(const char* ssid);

/**
 * Validate WiFi password
 * @param password WiFi password
 * @return 1 if valid, 0 otherwise
 */
int validate_password(const char* password);

/**
 * Validate IP address format
 * @param ip IP address string
 * @return 1 if valid, 0 otherwise
 */
int validate_ip_address(const char* ip);

/**
 * Validate JSON size is within limits
 * @param json_str JSON string
 * @return 1 if valid, 0 otherwise
 */
int validate_json_size(const char* json_str);

/**
 * Validate required fields in JSON object
 * @param json JSON object
 * @param field_names Array of required field names
 * @param field_count Number of required fields
 * @return ESP_OK if all fields present, ESP_ERR_INVALID_ARG otherwise
 */
esp_err_t validate_json_fields(cJSON* json, const char** field_names, int field_count);

#endif /* ESMESH_VALIDATION_H */
