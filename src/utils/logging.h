#ifndef ESMESH_LOGGING_H
#define ESMESH_LOGGING_H

#include <esp_log.h>

#define TAG "[EsMesh]"

/* Structured log macros */
#define LOG_BOOT(format, ...) ESP_LOGI("[BOOT]", format, ##__VA_ARGS__)
#define LOG_WIFI(format, ...) ESP_LOGI("[WIFI]", format, ##__VA_ARGS__)
#define LOG_HTTP(format, ...) ESP_LOGI("[HTTP]", format, ##__VA_ARGS__)
#define LOG_WS(format, ...) ESP_LOGI("[WS]", format, ##__VA_ARGS__)
#define LOG_MDNS(format, ...) ESP_LOGI("[MDNS]", format, ##__VA_ARGS__)
#define LOG_CHAT(format, ...) ESP_LOGI("[CHAT]", format, ##__VA_ARGS__)
#define LOG_CONFIG(format, ...) ESP_LOGI("[CONFIG]", format, ##__VA_ARGS__)
#define LOG_DIAG(format, ...) ESP_LOGI("[DIAG]", format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) ESP_LOGE("[ERROR]", format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) ESP_LOGW("[WARN]", format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) ESP_LOGD("[DEBUG]", format, ##__VA_ARGS__)

#endif /* ESMESH_LOGGING_H */
