#pragma once

#include <Arduino.h>
#include <time.h>

static const size_t kMaxHourlyEntries = 48;

struct WeatherData {
  bool valid = false;
  float temperatureC = NAN;
  float windspeed = NAN;
  float winddirection = NAN;
  int weathercode = -1;
  String timeIso;
  unsigned long fetchedAtMs = 0;

  size_t hourlyCount = 0;
  float hourlyApparent[kMaxHourlyEntries];
  float hourlyPrecipProb[kMaxHourlyEntries];
  time_t hourlyTimestamps[kMaxHourlyEntries];
};

// Fetches weather data for the current coordinates stored in WeatherSettings.
// Returns true when fresh (or cached) data is available; false on error and
// writes a human-readable message to 'message'.
bool weatherGetData(WeatherData& data, String& message);

// Clears any cached weather data (e.g. after coordinates change).
void weatherInvalidateCache();
