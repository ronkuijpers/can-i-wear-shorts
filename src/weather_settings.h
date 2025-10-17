#pragma once

#include <Preferences.h>
#include <Arduino.h>

class WeatherSettings {
public:
  void begin();

  const String& getLocationName() const { return locationName; }
  const String& getLatitude() const { return latitude; }
  const String& getLongitude() const { return longitude; }
  void setLocation(const String& name,
                   const String& lat,
                   const String& lon);

private:
  void persist();

  Preferences prefs;
  String locationName;
  String latitude;
  String longitude;
};

extern WeatherSettings weatherSettings;
