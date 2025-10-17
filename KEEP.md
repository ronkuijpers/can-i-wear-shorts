# KEEP – Functies die ongewijzigd kunnen blijven

## Netwerk & OTA
- `src/network_init.h` → `initNetwork()`
- `src/network.cpp` → `resetWiFiSettings()`
- `src/ota_init.h` → `initOTA()`
- `src/ota_updater.cpp` → OTA helperfuncties voor firmware/UI (`syncFilesFromManifest()`, `checkForFirmwareUpdate()` en hulpfuncties) zolang downloadtargets worden bijgewerkt.

## Logging & Systeem
- `src/log.cpp` / `src/log.h` → loggingbuffers, `setLogLevel()`, `initLogSettings()`
- `src/time_sync.h` / `src/time_sync.cpp` → tijdsynchronisatie (`initTimeSync()`)
- `src/mqtt_init.h` → `initMqtt()`, `mqttEventLoop()` (mits MQTT behouden blijft)
- `src/webserver_init.h` → `initWebServer()` (routing wordt elders aangepast, maar de init-call kan gelijk blijven)

## LED-aansturing
- `src/led_controller.cpp` / `src/led_controller.h` → `initLeds()`, `showLeds()` voor fysieke strip-aansturing
- `src/led_state.h` / `src/led_state.cpp` → `LedState`-klasse voor opslag van kleuren/helderheid

## Authenticatie & Preferences
- `src/ui_auth.h` → `UiAuth`-klasse (init, wachtwoordbeheer)
- `src/mqtt_settings.cpp` / `src/mqtt_settings.h` → opslaghelpers (`mqtt_settings_load()`, `mqtt_settings_save()`) los van inhoudelijke defaults

## PlatformIO & Build
- `platformio.ini` basisconfiguratie (board, framework, libraries) kan gelijk blijven; alleen namen/metadata bijwerken.
