#include "weather_client.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "log.h"
#include "secrets.h"
#include "weather_settings.h"

namespace {
constexpr unsigned long kCacheDurationMs = 10UL * 60UL * 1000UL;  // 10 minutes
WeatherData g_cache;
unsigned long g_lastFetchMs = 0;
String g_lastMessage;

String buildRequestUrl(const String& lat, const String& lon) {
  String url = WEATHER_API_ENDPOINT;
  if (url.startsWith("https://")) {
    url = "http://" + url.substring(8);
  }
  if (url.indexOf('?') < 0) {
    url += '?';
  } else if (!url.endsWith("&") && !url.endsWith("?")) {
    url += '&';
  }
  url += "latitude=" + lat;
  url += "&longitude=" + lon;
  url += "&current_weather=true";
  url += "&timezone=Europe%2FAmsterdam";
  url += "&hourly=apparent_temperature,precipitation_probability";
  url += "&forecast_days=2";
  return url;
}

time_t parseLocalIso(const char* iso) {
  if (!iso) return 0;
  int year, month, day, hour, minute;
  if (sscanf(iso, "%d-%d-%dT%d:%d", &year, &month, &day, &hour, &minute) != 5) {
    return 0;
  }
  struct tm tm {};
  tm.tm_year = year - 1900;
  tm.tm_mon = month - 1;
  tm.tm_mday = day;
  tm.tm_hour = hour;
  tm.tm_min = minute;
  tm.tm_sec = 0;
  tm.tm_isdst = -1;
  time_t value = mktime(&tm);
  return value >= 0 ? value : 0;
}

void assignCache(const WeatherData& data, const String& message) {
  g_cache = data;
  g_lastFetchMs = millis();
  g_lastMessage = message;
}
}  // namespace

bool weatherGetData(WeatherData& data, String& message) {
  const unsigned long now = millis();
  if (g_cache.valid && (now - g_lastFetchMs) < kCacheDurationMs) {
    data = g_cache;
    message = g_lastMessage;
    return true;
  }

  String lat = weatherSettings.getLatitude();
  String lon = weatherSettings.getLongitude();
  lat.trim();
  lon.trim();
  if (lat.isEmpty() || lon.isEmpty()) {
    message = "Geen coördinaten opgeslagen.";
    WeatherData empty;
    empty.valid = false;
    assignCache(empty, message);
    data = empty;
    return false;
  }

  if (WiFi.status() != WL_CONNECTED) {
    message = "Geen WiFi-verbinding.";
    WeatherData empty;
    empty.valid = false;
    assignCache(empty, message);
    data = empty;
    return false;
  }

  HTTPClient http;
  WiFiClient client;
  const String url = buildRequestUrl(lat, lon);
  logInfo(String("🌦️ Weather fetch from Open-Meteo: ") + url);
  http.setReuse(false);
  http.setTimeout(15000);
  http.useHTTP10(true);
  if (!http.begin(client, url)) {
    message = "HTTP begin failed";
    WeatherData empty;
    empty.valid = false;
    assignCache(empty, message);
    data = empty;
    return false;
  }

  const int code = http.GET();
  if (code <= 0) {
    message = "HTTP client error: " + http.errorToString(code);
    http.end();
    WeatherData empty;
    empty.valid = false;
    assignCache(empty, message);
    data = empty;
    return false;
  }
  if (code != 200) {
    message = String("HTTP status ") + code;
    http.end();
    WeatherData empty;
    empty.valid = false;
    assignCache(empty, message);
    data = empty;
    return false;
  }

  const String payload = http.getString();
  http.end();

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    message = String("JSON parse failed: ") + err.c_str();
    WeatherData empty;
    empty.valid = false;
    assignCache(empty, message);
    data = empty;
    return false;
  }

  JsonObject current = doc["current_weather"];
  if (current.isNull()) {
    message = "Geen 'current_weather' veld in antwoord.";
    WeatherData empty;
    empty.valid = false;
    assignCache(empty, message);
    data = empty;
    return false;
  }

  WeatherData fresh;
  fresh.valid = true;
  fresh.temperatureC = current["temperature"] | NAN;
  fresh.windspeed = current["windspeed"] | NAN;
  fresh.winddirection = current["winddirection"] | NAN;
  fresh.weathercode = current["weathercode"] | -1;
  fresh.timeIso = current["time"].as<String>();
  fresh.fetchedAtMs = millis();
  fresh.hourlyCount = 0;

  JsonObject hourly = doc["hourly"];
  if (!hourly.isNull()) {
    JsonArray times = hourly["time"].as<JsonArray>();
    JsonArray temps = hourly["apparent_temperature"].as<JsonArray>();
    JsonArray probs = hourly["precipitation_probability"].as<JsonArray>();
    if (!times.isNull() && !temps.isNull() && !probs.isNull()) {
      size_t count = std::min({static_cast<size_t>(times.size()),
                               static_cast<size_t>(temps.size()),
                               static_cast<size_t>(probs.size()),
                               kMaxHourlyEntries});
      for (size_t i = 0; i < count; ++i) {
        const char* timeStr = times[i];
        fresh.hourlyTimestamps[i] = parseLocalIso(timeStr);
        fresh.hourlyApparent[i] = temps[i].isNull() ? NAN : temps[i].as<float>();
        fresh.hourlyPrecipProb[i] = probs[i].isNull() ? NAN : probs[i].as<float>();
      }
      fresh.hourlyCount = count;
    }
  }

  message = "OK";
  assignCache(fresh, message);
  data = fresh;
  return true;
}

void weatherInvalidateCache() {
  g_cache.valid = false;
  g_lastFetchMs = 0;
  g_lastMessage = "";
  g_cache.hourlyCount = 0;
}
