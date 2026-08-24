#ifndef ESMESH_MESSAGE_H
#define ESMESH_MESSAGE_H

#include <stdint.h>
#include <cJSON.h>
#include <esp_err.h>

/* Message structure for internal representation */
typedef struct {
    char id[33];                    /* Unique message ID */
    char source[33];                /* Source node ID or "PHONE" */
    char destination[33];           /* Destination node ID or "PHONE" */
    uint32_t timestamp;             /* Unix timestamp */
    uint8_t ttl;                    /* Time-to-live */
    char payload[2049];             /* Message payload */
    uint32_t received_at;           /* When we received it (internal) */
} esmesh_message_t;

/**
 * Parse and validate a message from JSON
 * @param json_str JSON string from network
 * @param msg Output message structure
 * @return ESP_OK on success, ESP_ERR_* on failure
 */
esp_err_t esmesh_message_from_json(const char* json_str, esmesh_message_t* msg);

/**
 * Convert message to JSON string
 * @param msg Message structure
 * @return char* allocated JSON string (must be freed) or NULL
 */
char* esmesh_message_to_json(const esmesh_message_t* msg);

/**
 * Generate a unique message ID
 * @param id_out Buffer for ID (min 33 bytes)
 * @return ESP_OK on success
 */
esp_err_t esmesh_generate_msg_id(char* id_out);

/**
 * Check if message ID is valid (not empty, reasonable length)
 * @param id Message ID to validate
 * @return 1 if valid, 0 otherwise
 */
int esmesh_is_valid_msg_id(const char* id);

/**
 * Check if payload is valid (not empty, under size limit)
 * @param payload Payload string
 * @return 1 if valid, 0 otherwise
 */
int esmesh_is_valid_payload(const char* payload);

/**
 * Check if node ID is valid
 * @param node_id Node ID to check
 * @return 1 if valid, 0 otherwise
 */
int esmesh_is_valid_node_id(const char* node_id);

#endif /* ESMESH_MESSAGE_H */
