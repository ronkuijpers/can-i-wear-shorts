#include "weather_settings.h"
#include "secrets.h"
#include "weather_client.h"

WeatherSettings weatherSettings;

void WeatherSettings::begin() {
  prefs.begin("weather", false);
  locationName = prefs.getString("name", "");
  latitude = prefs.getString("lat", WEATHER_LATITUDE);
  longitude = prefs.getString("lon", WEATHER_LONGITUDE);
  prefs.end();
}

void WeatherSettings::setLocation(const String& name,
                                  const String& lat,
                                  const String& lon) {
  locationName = name;
  latitude = lat;
  longitude = lon;
  persist();
}

void WeatherSettings::persist() {
  prefs.begin("weather", false);
  prefs.putString("name", locationName);
  prefs.putString("lat", latitude);
  prefs.putString("lon", longitude);
  prefs.end();
  weatherInvalidateCache();
}
