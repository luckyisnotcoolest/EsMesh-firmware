# EsMesh ESP32-S3 Firmware

A complete, stable mesh networking firmware for ESP32-S3 microcontrollers with real-time communication, local network discovery, and web-based device management.

## Features

- **WiFi Station Mode**: Connects to 2.4 GHz WiFi router
- **REST API**: Full EsMesh/1 protocol implementation
- **WebSocket Server**: Real-time bidirectional messaging
- **mDNS Discovery**: Automatic device discovery via `_espmesh._tcp.local`
- **Web UI**: Modern dashboard with chat interface, device management, and diagnostics
- **Persistent Configuration**: Non-volatile storage for settings
- **Real Device Reporting**: Actual chip specs, memory, WiFi state—no fake values
- **Security**: Input validation, resource limits, safe memory management

## Hardware Support

- ESP32-S3 N16R8 (16MB flash, 8MB PSRAM)
- ESP32-S3 N8R2 (8MB flash, 2MB PSRAM)

## Architecture

```
Android App          WiFi Router
    ↓                    ↓
    └────── LAN ────────┬────┘
                        ↓
                    ESP32-S3
                  (This Firmware)
                  
Core Components:
  • WiFi Stack (STA, DHCP, static IP)
  • HTTP Server (REST API)
  • WebSocket Server (real-time messaging)
  • mDNS Service (discovery)
  • Configuration Storage (NVS)
  • Device Identity (stable across reboot)
  • Web UI (static + JavaScript)
```

## Build System

**ESP-IDF 5.2+** with CMake

### Prerequisites

```bash
# Install ESP-IDF (if not already)
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32s3
. ./export.sh
```

### Build for ESP32-S3 N16R8

```bash
idf.py set-target esp32s3
idf.py menuconfig
# Configure target (ESP32-S3, 16MB flash)
idf.py build
idf.py flash -p /dev/ttyUSB0
idf.py monitor -p /dev/ttyUSB0
```

### Build for ESP32-S3 N8R2

```bash
idf.py set-target esp32s3
idf.py menuconfig
# Configure target (ESP32-S3, 8MB flash, 2MB PSRAM)
idf.py build
```

## Project Structure

```
EsMesh-firmware/
├── CMakeLists.txt              # Root build configuration
├── README.md                   # This file
├── src/
│   ├── main/
│   │   └── main.c              # Entry point, boot sequence
│   ├── core/
│   │   ├── device_id.c         # Device identity management
│   │   ├── device_id.h
│   │   ├── capabilities.c       # Hardware capability detection
│   │   └── capabilities.h
│   ├── wifi/
│   │   ├── wifi_manager.c       # WiFi STA mode, reconnect logic
│   │   └── wifi_manager.h
│   ├── http/
│   │   ├── http_server.c        # HTTP server setup
│   │   └── http_server.h
│   ├── api/
│   │   ├── api_handlers.c       # REST API endpoints
│   │   ├── api_handlers.h
│   │   ├── api_status.c         # /api/v1/status
│   │   ├── api_config.c         # /api/v1/config
│   │   ├── api_messages.c       # /api/v1/messages
│   │   └── api_utils.c          # JSON building, error responses
│   ├── websocket/
│   │   ├── ws_server.c          # WebSocket server
│   │   └── ws_server.h
│   ├── mdns/
│   │   ├── mdns_service.c       # mDNS discovery
│   │   └── mdns_service.h
│   ├── protocol/
│   │   ├── message.c            # Message validation & processing
│   │   ├── message.h
│   │   ├── protocol.h           # Protocol constants (EsMesh/1)
│   │   └── validation.c         # Input validation
│   ├── storage/
│   │   ├── config_storage.c     # NVS-based configuration
│   │   └── config_storage.h
│   └── utils/
│       ├── logging.c            # Structured logging
│       ├── logging.h
│       └── json_builder.c        # JSON response building
├── web/
│   ├── index.html               # Main web interface
│   ├── style.css                # Responsive styling (black, red, yellow)
│   └── app.js                   # Client-side logic (WiFi state, chat, config)
├── test/
│   ├── test_message.c           # Unit tests for message validation
│   ├── test_protocol.c          # Protocol compliance tests
│   └── test_config.c            # Configuration tests
├── docs/
│   ├── API.md                   # REST API reference
│   ├── WEBSOCKET.md             # WebSocket protocol spec
│   ├── DISCOVERY.md             # mDNS discovery guide
│   ├── CONFIGURATION.md         # Configuration system
│   ├── CAPABILITIES.md          # Capability detection
│   ├── ANDROID_COMPATIBILITY.md # Protocol compatibility matrix
│   ├── HARDWARE_TEST_PLAN.md    # Testing procedures
│   ├── BUILD.md                 # Detailed build instructions
│   └── TROUBLESHOOTING.md       # Common issues and fixes
├── .github/
│   └── workflows/
│       └── build.yml            # GitHub Actions CI/CD
└── sdkconfig.defaults           # Default ESP-IDF configuration
```

## REST API Overview

All endpoints return JSON with protocol version, status, and payload.

### Status
```
GET /api/v1/status
→ { device_id, ip, rssi, uptime, status, ... }
```

### Capabilities
```
GET /api/v1/capabilities
→ { protocol: "EsMesh/1", device: "ESP32-S3", capabilities: {...} }
```

### Device Info
```
GET /api/v1/device
→ { name, id, chip_model, firmware_version, flash_size, psram_size, ... }
```

### Network Info
```
GET /api/v1/network
→ { wifi_ssid, ip, gateway, subnet, dns, rssi, mac_address, ... }
```

### Messages (History)
```
GET /api/v1/messages
→ { messages: [{ id, source, destination, payload, timestamp }, ...] }
```

### Send Message
```
POST /api/v1/message
{ source, destination, payload, ttl }
→ { id, status: "sent", timestamp }
```

### Configuration
```
GET /api/v1/config
→ { device_name, wifi_ssid, wifi_ip, wifi_gateway, ... }

POST /api/v1/config
{ device_name, wifi_ssid, ... }
→ { status: "saved", requires_restart: bool }
```

### Restart
```
POST /api/v1/restart
→ { status: "restarting" }
```

### WiFi Test
```
POST /api/v1/wifi/test
{ ssid, password }
→ { success: bool, rssi: int, message: string }
```

## WebSocket Protocol

**Endpoint**: `ws://DEVICE-IP/api/v1/ws`

**Message Format**:
```json
{
  "protocol": "EsMesh/1",
  "type": "message|ping|pong|config",
  "id": "unique-id",
  "source": "NODE-A",
  "destination": "PHONE",
  "timestamp": 1234567890,
  "payload": "message content",
  "ttl": 5
}
```

## Configuration Storage

Persistent settings stored in NVS (Non-Volatile Storage):
- Device name
- WiFi SSID & password
- Static IP settings (optional)
- AP mode settings (if supported)
- Device ID (stable)

## Device Identity

Stable device ID (e.g., `ESM-A1B2C3`) generated on first boot and stored persistently. Remains unchanged across normal reboots.

## Logging

Structured log format:
```
[EsMesh][BOOT] Starting firmware...
[EsMesh][WIFI] Connecting to SSID...
[EsMesh][HTTP] Server started on port 80
[EsMesh][WS] Client connected (1 connected)
[EsMesh][ERROR] Invalid JSON received
```

## Security

- **No hard-coded secrets**: WiFi passwords stored only in NVS
- **Input validation**: All network input sanitized
- **Resource limits**: Bounded message queues, JSON sizes, WebSocket clients
- **Memory safety**: No unbounded allocations from network input
- **Logging**: Never logs passwords or authentication tokens

## Testing

See `docs/HARDWARE_TEST_PLAN.md` for comprehensive testing procedures.

**Available Tests**:
- Unit tests (JSON parsing, message validation)
- Integration tests (HTTP, WebSocket, mDNS)
- Hardware tests (WiFi, DHCP, reconnect logic)

## Known Limitations

- **AP Mode**: Not yet implemented (capability reported as false)
- **Mesh Routing**: Single-node only (no mesh relay)
- **Bluetooth**: Not yet implemented
- **OTA Updates**: Not yet implemented (capability reported as false)

## Hardware Testing

**Status**: NOT PERFORMED

This firmware has been designed to actual ESP32-S3 specifications and built against official ESP-IDF. Physical hardware testing is required before production deployment.

## License

MIT License (to be configured)

## Support

For issues, questions, or contributions, open an issue in the repository.
