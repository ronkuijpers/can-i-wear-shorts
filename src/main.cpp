#include "webserver_init.h"
#include "mqtt_init.h"
#include "display_init.h"
#include "startup_sequence_init.h"
#include "clothing_display_loop.h"
#include "time_sync.h"
#include "clothing_system_init.h"

// Can I Wear Shorts hoofdprogramma (voorlopig nog met legacy clothing-display loop)
// - Setup: initialiseert hardware, netwerk, OTA, filesystem en start services
// - Loop: verwerkt webrequests, OTA, MQTT en de huidige LED-logica

#include <Arduino.h>
#include <ESPmDNS.h>
#include "fs_compat.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <WiFiServer.h>
#include <time.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include "clothing_display.h"
#include "web_routes.h"
#include "network_init.h"
#include "log.h"
#include "config.h"
#include "ota_init.h"
#include "sequence_controller.h"
#include "display_settings.h"
#include "weather_settings.h"
#include "ui_auth.h"
#include "mqtt_client.h"


bool clockEnabled = true;
StartupSequence startupSequence;
DisplaySettings displaySettings;
UiAuth uiAuth;


// Webserver
WebServer server(80);

// Tracking (handled inside loop as statics)

// Setup: initialiseert hardware, netwerk, OTA, filesystem en start de hoofdservices
void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  delay(MDNS_START_DELAY_MS);
  initLogSettings();

  initNetwork();              // WiFiManager (WiFi-instellingen en verbinding)
  initOTA();                  // OTA (Over-the-air updates)

  // Start mDNS voor lokale netwerknaam
  if (MDNS.begin(MDNS_HOSTNAME)) {
    logInfo("🌐 mDNS active at http://" MDNS_HOSTNAME ".local");
  } else {
    logError("❌ mDNS start failed");
  }

  // Load persisted display and weather settings before running dependent flows
  displaySettings.begin();
  weatherSettings.begin();

  // Mount SPIFFS filesystem
  if (!FS_IMPL.begin(true)) {
  logError("SPIFFS mount failed.");
  } else {
  logDebug("SPIFFS loaded successfully.");
  }

  initWebServer(server);      // Webserver en routes

  // Wacht op WiFi verbinding (max 20x proberen)
  logInfo("Checking WiFi connection");
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < WIFI_CONNECT_MAX_RETRIES) {
    delay(WIFI_CONNECT_RETRY_DELAY_MS);
    logInfo(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    initMqtt();
    if (displaySettings.getAutoUpdate()) {
  logInfo("✅ Connected to WiFi. Starting firmware check...");
      checkForFirmwareUpdate();
    } else {
  logInfo("ℹ️ Automatic firmware updates disabled. Skipping check.");
    }
  } else {
  logInfo("⚠️ No WiFi. Firmware check skipped.");
  }

  // Synchroniseer tijd via NTP
  initTimeSync(TZ_INFO, NTP_SERVER1, NTP_SERVER2);
  initDisplay();
  initClothingSystem(uiAuth);
  initStartupSequence(startupSequence);
}

// Loop: hoofdprogramma, verwerkt webrequests, OTA, MQTT en kloklogica
void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  mqttEventLoop();

  // Startup animatie: blokkeert klok tot animatie klaar is
  if (updateStartupSequence(startupSequence)) {
    return;  // Voorkomt dat klok al tijd toont
  }

  // Tijd- en animatie-update (clothingDisplayLoop regelt zelf per-minuut/animatie)
  static unsigned long lastLoop = 0;
  unsigned long now = millis();
  if (now - lastLoop >= 50) {
    lastLoop = now;
    runClothingDisplayLoop();

    // Dagelijkse firmwarecheck om 02:00
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      time_t nowEpoch = time(nullptr);
      static time_t lastFirmwareCheck = 0;
      if (timeinfo.tm_hour == 2 && timeinfo.tm_min == 0 && nowEpoch - lastFirmwareCheck > 3600) {
        if (displaySettings.getAutoUpdate()) {
          logInfo("🛠️ Daily firmware check started...");
          checkForFirmwareUpdate();
        } else {
          logInfo("ℹ️ Automatic firmware updates disabled (02:00 check skipped)");
        }
        lastFirmwareCheck = nowEpoch;
      }
    }
  }
}
