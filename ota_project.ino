#include "wifi_config.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>
#include <Ticker.h>


// Wi-Fi credentials (defined in wifi_config.h)
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Firmware update URL
const char* firmware_url = "https://github.com/<github_username>/<repo_name>/releases/latest/download/firmware.bin";

// Update check interval (in msecs)
const unsigned long updateInterval = 60 * 60 * 1000;  // 1 hour
unsigned long lastUpdateCheck = 0;    // Timestamp of last update check


// Watchdog timer
Ticker watchdog;
bool updateInProgress = false;

// Reset watchdog timer
void resetWatchdog() {
  ESP.wdtFeed();
}

// Check for OTA updates
void checkForUpdates() {
  if (updateInProgress) return;   // Skip if already updating

  Serial.println("Checking for updates...");
  HTTPClient http;
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(firmware_url);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Update available, starting OTA...");
      updateInProgress = true;
      watchdog.detach();   // Disable watchdog during update

      t_httpUpdate_return ret = ESPhttpUpdate.update(http.getStream(), firmware_url);
      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("Update failed. Error (%d): %s\n",
                        ESPhttpUpdate.getLastError(),
                        ESPhttpUpdate.getLastErrorString().c_str());
          updateInProgress = false;
          watchdog.attach(2, resetWatchdog);   // Re-enable watchdog
          break;

        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("No updates available.");
          updateInProgress = false;
          watchdog.attach(2, resetWatchdog);   // Re-enable watchdog
          break;

        case HTTP_UPDATE_OK:
          Serial.println("Update successful. Rebooting...");
          break;   // Device will reboot after successful update
      }
    } else {
      Serial.printf("HTTP GET failed. Error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("Wi-Fi not connected. Skipping update check.");
  }
}


void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Watchdog
  ESP.wdtEnable(WDTO_8S);   // 8-second timeout
  watchdog.attach(2, resetWatchdog);  // Feed watchdog every 2 seconds

  // Initialize OTA for updates
  ArduinoOTA.onStart([]() {
    String type = ArduinoOTA.getCommand() == U_FLASH ? "sketch" : "filesystem";
    Serial.println("Start updating " + type);
    updateInProgress = true;
    watchdog.detach();   // Temporarily disable watchdog during OTA
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nUpdate Complete");
    ESP.restart();   // Restart device after update
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    ESP.restart();   // Restart device to recover
  });

  ArduinoOTA.begin();
  Serial.println("OTA Ready");

  // Initial update check
  checkForUpdates();
}


void loop() {
  ArduinoOTA.handle();   // Handle OTA updates from Arduino CLI -or- IDE

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateCheck >= updateInterval) {
    lastUpdateCheck = currentMillis;
    checkForUpdates();   // Trigger periodic update check
  }


  // -INSERT PROJECT FUNCTIONALITY CODE HERE-

}
