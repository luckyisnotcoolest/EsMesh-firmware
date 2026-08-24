#ifndef ESMESH_WIFI_MANAGER_H
#define ESMESH_WIFI_MANAGER_H

#include <esp_err.h>
#include <esp_wifi_types.h>

/**
 * WiFi status states
 */
typedef enum {
    WIFI_STATUS_DISCONNECTED = 0,
    WIFI_STATUS_CONNECTING = 1,
    WIFI_STATUS_CONNECTED = 2,
    WIFI_STATUS_ERROR = 3
} wifi_status_t;

/**
 * Initialize WiFi subsystem
 * Sets up WiFi event handlers
 * @return ESP_OK on success
 */
esp_err_t wifi_manager_init(void);

/**
 * Start WiFi connection to configured SSID
 * Non-blocking; uses internal retry/backoff
 * @return ESP_OK if connection started
 */
esp_err_t wifi_manager_start(void);

/**
 * Stop WiFi connection
 * @return ESP_OK on success
 */
esp_err_t wifi_manager_stop(void);

/**
 * Get current WiFi connection status
 * @return wifi_status_t
 */
wifi_status_t wifi_manager_get_status(void);

/**
 * Get current RSSI (signal strength)
 * @return RSSI in dBm (negative value), or 0 if not connected
 */
int32_t wifi_manager_get_rssi(void);

/**
 * Get connected SSID
 * @param ssid_out Buffer for SSID string (min 33 bytes)
 * @return ESP_OK on success
 */
esp_err_t wifi_manager_get_connected_ssid(char* ssid_out);

/**
 * Get device IP address
 * @param ip_out Buffer for IP string (min 16 bytes, e.g., "192.168.1.100")
 * @return ESP_OK on success
 */
esp_err_t wifi_manager_get_ip(char* ip_out);

/**
 * Get gateway IP
 * @param gw_out Buffer for gateway IP string (min 16 bytes)
 * @return ESP_OK on success
 */
esp_err_t wifi_manager_get_gateway(char* gw_out);

/**
 * Get subnet mask
 * @param subnet_out Buffer for subnet string (min 16 bytes)
 * @return ESP_OK on success
 */
esp_err_t wifi_manager_get_subnet(char* subnet_out);

/**
 * Get MAC address
 * @param mac_out Buffer for MAC string (min 18 bytes, "XX:XX:XX:XX:XX:XX")
 * @return ESP_OK on success
 */
esp_err_t wifi_manager_get_mac(char* mac_out);

/**
 * Test WiFi connection with supplied credentials
 * Does not modify current configuration
 * @param ssid SSID to test
 * @param password Password to test
 * @param timeout_ms How long to wait for connection
 * @return ESP_OK if connection succeeded, ESP_FAIL otherwise
 */
esp_err_t wifi_manager_test_connection(const char* ssid, const char* password, uint32_t timeout_ms);

/**
 * Reconfigure WiFi with new credentials
 * Disconnects current connection and connects to new SSID
 * @param ssid New SSID
 * @param password New password
 * @return ESP_OK if reconfiguration started
 */
esp_err_t wifi_manager_reconfigure(const char* ssid, const char* password);

#endif /* ESMESH_WIFI_MANAGER_H */
