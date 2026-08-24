#ifndef ESMESH_HTTP_SERVER_H
#define ESMESH_HTTP_SERVER_H

#include <esp_err.h>
#include <esp_http_server.h>

/**
 * Initialize and start HTTP server
 * Registers all API endpoints and static file handlers
 * @return ESP_OK on success
 */
esp_err_t http_server_init(void);

/**
 * Stop and clean up HTTP server
 * @return ESP_OK on success
 */
esp_err_t http_server_stop(void);

/**
 * Get HTTP server handle
 * @return httpd_handle_t or NULL
 */
httpd_handle_t http_server_get_handle(void);

/**
 * Check if server is running
 * @return 1 if running, 0 otherwise
 */
int http_server_is_running(void);

#endif /* ESMESH_HTTP_SERVER_H */
