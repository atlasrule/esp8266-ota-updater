# ESP8266 OTA Firmware Updater


Firmware updater for Wemos D1 Mini (ESP8266) featuring Over-The-Air (OTA) updates, dual-partition fail-safe mechanism, watchdog timer integration, and remote update capabilities. 
Ideal for IoT devices in hard to reach locations (e.g. my roof).
By leveraging GitHub Actions, it automates the build process and hosts the `firmware.bin` file, making it easy to update your devices wirelessly.



## Features

- **OTA Updates**: Update ESP8266 firmware over Wi-Fi.
- **GitHub Actions Integration**: Automatically builds and hosts the `firmware.bin` file on GitHub Releases.
- **Fail-Safe Mechanism**: Includes watchdog support for enhanced stability.
- **Automation**: Simplified setup for deploying updates remotely.

<br>

---
<br>


## How It Works

1. The ESP8266 fetches the `firmware.bin` file from the **GitHub Releases** page using a pre-configured URL.
2. GitHub Actions compiles the code and uploads the `firmware.bin` file whenever changes are pushed to the `main` branch.
3. The device downloads and applies the update via OTA.

<br>

---
<br>


## Structure

```
├── esp8266_ota_project/
│   └── esp8266_ota_project.ino   # Main Arduino code file
├── .gitignore
├── .github/
│   └── workflows/
│       └── build-firmware.yml    # GitHub Actions workflow for building firmware
└── README.md                     # Project documentation
```

<br>

---
<br>


## Prerequisites

1. **Hardware**:
   - ESP8266-based device (e.g., Wemos D1 Mini, NodeMCU, or other compatible boards).

2. **Software**:
   - Arduino IDE or Arduino CLI (for remote building).
   - GitHub account for repository and Actions integration.

<br>

---
<br>


## Getting Started

### Step 1: Clone the Repository

Clone the repository to your local machine:

```bash
git clone https://github.com/<github_username>/<repo_name>.git
cd <repo_name>
```

### Step 2: Set Up GitHub Secrets

To securely store your Wi-Fi credentials:

1. Go to your GitHub repository.
2. Navigate to **Settings** > **Secrets and variables** > **Actions**.
3. Add the following secrets:
   - **`WIFI_SSID`**: Your Wi-Fi SSID.
   - **`WIFI_PASSWORD`**: Your Wi-Fi password.

These secrets will be injected into the code during the GitHub Actions build process.

### Step 3: Configure the `wifi_config.h` File

Ensure your `wifi_config.h` file contains placeholders for the credentials:

```cpp
#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#define WIFI_SSID "<SSID_PLACEHOLDER>"
#define WIFI_PASSWORD "<PASSWORD_PLACEHOLDER>"

#endif
```

The build process will automatically replace `<SSID_PLACEHOLDER>` and `<PASSWORD_PLACEHOLDER>` with the values from your GitHub Secrets.

### Step 4: Flash Initial Firmware Locally

If this is the first time deploying the firmware, you need to flash it locally:

1. Open the project in **Arduino IDE** or use **Arduino CLI**.
2. Select the appropriate board (`Generic ESP8266 Module` or another ESP8266-based board).
3. Temporarily replace the placeholders in `wifi_config.h` with your Wi-Fi credentials:
   ```cpp
   #define WIFI_SSID "your_actual_SSID"
   #define WIFI_PASSWORD "your_actual_password"
   ```
4. Flash the firmware to your ESP8266 device using a USB connection.

Once the initial firmware is flashed, OTA updates will handle subsequent changes.

### Step 5: Push Changes to GitHub

After setting up your sanitized `wifi_config.h` and other code changes:

1. Ensure `wifi_config.h` is added to `.gitignore` to avoid accidental commits:
   ```plaintext
   wifi_config.h
   ```
2. Commit and push your changes:
   ```bash
   git add .
   git commit -m "Initial commit"
   git push origin main
   ```

### Step 6: Verify GitHub Actions Workflow

1. Go to the **Actions** tab in your GitHub repository.
2. Confirm the `Build Firmware` workflow runs successfully.
3. Check the **Releases** tab to ensure the `firmware.bin` file is available for download.

### Step 7: Test OTA Updates

1. Power on your ESP8266 device and ensure it is connected to the specified Wi-Fi network.
2. The device will automatically fetch and apply the latest `firmware.bin` file from GitHub Releases.

<br>

---
<br>


## Using GitHub Actions

### Workflow Overview

The `.github/workflows/build-firmware.yml` file automates the following steps:

1. **Trigger on Push**: Automatically runs on any push to the `main` branch.
2. **Compile Firmware**: Uses Arduino CLI to build the firmware for ESP8266 devices.
3. **Upload Binary**: Publishes the compiled `firmware.bin` to GitHub Releases.

<br>

---
<br>


## Updating Firmware

To update the firmware on your ESP8266:

1. Change the `esp8266_ota_project.ino` file.
2. Push your changes to GitHub:

```bash
git add .
git commit -m "Added new feature"
git push origin main
```

3. Wait for GitHub Actions to build the new firmware.
4. Reboot your ESP8266 to trigger the OTA update.

<br>

---
<br>


<br>
<br>
