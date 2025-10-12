// mqtt_settings.h
#pragma once

#include <Arduino.h>

struct MqttSettings {
  String host;
  uint16_t port = 1883;
  String user;   // optional
  String pass;   // optional
  String discoveryPrefix = "homeassistant";
  String baseTopic = "wordclock";
};

// Load from Preferences. If nothing stored, prefill from secrets.h macros if present.
bool mqtt_settings_load(MqttSettings& out);

// Save to Preferences (persist across reboots)
bool mqtt_settings_save(const MqttSettings& in);

