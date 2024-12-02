# ESP8266 OTA Firmware Updater

Firmware updater for Wemos D1 Mini (ESP8266) featuring Over-The-Air (OTA) updates, dual-partition fail-safe mechanism, watchdog timer integration, and remote update capabilities. 
Ideal for IoT devices in hard to reach locations (like my roof) (or like moon).
By leveraging GitHub Actions, it automates the build process and hosts the `firmware.bin` file, making it easy to update your devices wirelessly.


## Features

- **OTA Updates**: Update ESP8266 firmware over Wi-Fi.
- **GitHub Actions Integration**: Automatically builds and hosts the `firmware.bin` file on GitHub Releases.
- **Fail-Safe Mechanism**: Includes watchdog support for enhanced stability.
- **Automation**: Simplified setup for deploying updates remotely.


## How It Works

1. The ESP8266 fetches the `firmware.bin` file from the **GitHub Releases** page using a pre-configured URL.
2. GitHub Actions compiles the code and uploads the `firmware.bin` file whenever changes are pushed to the `main` branch.
3. The device downloads and applies the update via OTA.


## Prerequisites

1. **Hardware**:
   - ESP8266-based device (e.g., Wemos D1 Mini, NodeMCU, or other compatible boards).

2. **Software**:
   - Arduino IDE or Arduino CLI (for remote building).
   - GitHub account for repository and Actions integration.
