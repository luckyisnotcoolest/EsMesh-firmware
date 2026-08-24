#ifndef ESMESH_MDNS_SERVICE_H
#define ESMESH_MDNS_SERVICE_H

#include <esp_err.h>

/**
 * Initialize mDNS service
 * Registers _espmesh._tcp service for discovery
 * @param hostname Device hostname (without .local)
 * @return ESP_OK on success
 */
esp_err_t mdns_service_init(const char* hostname);

/**
 * Update mDNS service hostname and properties
 * @param hostname New hostname
 * @return ESP_OK on success
 */
esp_err_t mdns_service_update(const char* hostname);

/**
 * Stop mDNS service
 * @return ESP_OK on success
 */
esp_err_t mdns_service_stop(void);

#endif /* ESMESH_MDNS_SERVICE_H */
