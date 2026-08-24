#ifndef ESMESH_WS_SERVER_H
#define ESMESH_WS_SERVER_H

#include <esp_err.h>
#include <cJSON.h>

/**
 * Initialize WebSocket server
 * Registers WebSocket endpoint with HTTP server
 * @return ESP_OK on success
 */
esp_err_t ws_server_init(void);

/**
 * Send message to all connected WebSocket clients
 * @param json Message as cJSON
 * @return ESP_OK if sent to at least one client, ESP_FAIL otherwise
 */
esp_err_t ws_server_broadcast(cJSON* json);

/**
 * Send message to specific WebSocket client
 * @param session_id Client session identifier
 * @param json Message as cJSON
 * @return ESP_OK on success
 */
esp_err_t ws_server_send_to_client(uint32_t session_id, cJSON* json);

/**
 * Get count of connected WebSocket clients
 * @return Number of connected clients
 */
int ws_server_get_client_count(void);

/**
 * Check if WebSocket server is active
 * @return 1 if active, 0 otherwise
 */
int ws_server_is_active(void);

#endif /* ESMESH_WS_SERVER_H */
