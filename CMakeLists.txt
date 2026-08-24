# EsMesh ESP32-S3 Firmware - Root CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(EsMesh VERSION 1.0.0)

# Add main component
idf_component_register(
    SRCS 
        src/main/main.c
        src/core/device_id.c
        src/core/capabilities.c
        src/wifi/wifi_manager.c
        src/http/http_server.c
        src/api/api_handlers.c
        src/api/api_status.c
        src/api/api_config.c
        src/api/api_messages.c
        src/api/api_utils.c
        src/websocket/ws_server.c
        src/mdns/mdns_service.c
        src/protocol/message.c
        src/protocol/validation.c
        src/storage/config_storage.c
        src/utils/logging.c
        src/utils/json_builder.c
    INCLUDE_DIRS 
        src/
        src/main/
        src/core/
        src/wifi/
        src/http/
        src/api/
        src/websocket/
        src/mdns/
        src/protocol/
        src/storage/
        src/utils/
    REQUIRES 
        esp_wifi
        esp_http_server
        mdns
        nvs_flash
        esp_event
        lwip
        cjson
)
