#pragma once

#include <ArduinoOTA.h>
#include "log.h"
#include "secrets.h"

// Initialize OTA (Over-the-air updates)
// This function configures and starts the OTA service to allow firmware updates over the network.
// Callback functions provide logging and proper handling of the update process.
inline void initOTA() {
    ArduinoOTA.setHostname(OTA_HOSTNAME); // Set OTA hostname
    ArduinoOTA.setPassword(OTA_PASSWORD); // Set OTA password
    ArduinoOTA.setPort(OTA_PORT); // Set OTA port

    ArduinoOTA.onStart([]() {
    logInfo("🔄 Starting network OTA update");
    });
    ArduinoOTA.onEnd([]() {
        logInfo("✅ OTA update complete, restarting in 1s");
        delay(OTA_UPDATE_COMPLETE_DELAY_MS);
        ESP.restart();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        uint8_t pct = (progress * 100) / total;
        logInfo("📶 OTA Progress: " + String(pct) + "%");
    });
    ArduinoOTA.onError([](ota_error_t err) {
        String msg = "[OTA] Error: ";
        switch (err) {
            case OTA_AUTH_ERROR:    msg += "Authentication failed"; break;
            case OTA_BEGIN_ERROR:   msg += "Begin failed";         break;
            case OTA_CONNECT_ERROR: msg += "Connection failed";     break;
            case OTA_RECEIVE_ERROR: msg += "Receive failed";        break;
            case OTA_END_ERROR:     msg += "End failed";            break;
            default:                msg += "Unknown";               break;
        }
        logError(msg);
    });
    ArduinoOTA.begin();
    logInfo("🟢 Network OTA service active, waiting for upload");
}
