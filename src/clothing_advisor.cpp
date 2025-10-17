#include "clothing_advisor.h"

#include <algorithm>
#include <math.h>
#include <time.h>

namespace {
const char* kModeNow = "now";
const char* kModeToday = "today";

struct WindowResult {
  size_t indices[kMaxHourlyEntries];
  size_t count = 0;
};

time_t roundUpToNextHour(time_t ts) {
  if (ts <= 0) return 0;
  const time_t remainder = ts % 3600;
  if (remainder == 0) {
    return ts + 3600;
  }
  return ts + (3600 - remainder);
}

void collectNowWindow(const WeatherData& data, WindowResult& result) {
  time_t now = time(nullptr);
  time_t start = roundUpToNextHour(now);
  for (size_t i = 0; i < data.hourlyCount && result.count < 3; ++i) {
    time_t ts = data.hourlyTimestamps[i];
    if (ts >= start && ts > 0) {
      result.indices[result.count++] = i;
    }
  }
  // Fallback: if no future hours found, use the last known hours
  if (result.count == 0 && data.hourlyCount > 0) {
    size_t take = std::min<size_t>(3, data.hourlyCount);
    for (size_t i = data.hourlyCount - take; i < data.hourlyCount && result.count < kMaxHourlyEntries; ++i) {
      result.indices[result.count++] = i;
    }
  }
}

void collectRangeWindow(const WeatherData& data, time_t start, time_t end, WindowResult& result) {
  for (size_t i = 0; i < data.hourlyCount; ++i) {
    time_t ts = data.hourlyTimestamps[i];
    if (ts >= start && ts < end && ts > 0) {
      if (result.count < kMaxHourlyEntries) {
        result.indices[result.count++] = i;
      } else {
        break;
      }
    }
  }
}

void collectTodayWindow(const WeatherData& data, WindowResult& result) {
  time_t now = time(nullptr);
  struct tm localNow;
  localtime_r(&now, &localNow);

  struct tm startTm = localNow;
  struct tm endTm = localNow;

  if (localNow.tm_hour >= 21) {
    // Tomorrow 08:00 - 21:59
    time_t tomorrow = now + 24 * 3600;
    struct tm tmTomorrow;
    localtime_r(&tomorrow, &tmTomorrow);
    startTm = tmTomorrow;
    startTm.tm_hour = 8;
    startTm.tm_min = 0;
    startTm.tm_sec = 0;
  } else if (localNow.tm_hour < 8) {
    startTm.tm_hour = 8;
    startTm.tm_min = 0;
    startTm.tm_sec = 0;
  } else {
    startTm.tm_min = 0;
    startTm.tm_sec = 0;
  }

  int startHour = startTm.tm_hour;
  if (startHour < 8) startHour = 8;
  if (startHour > 21) startHour = 21;
  startTm.tm_hour = startHour;
  startTm.tm_min = 0;
  startTm.tm_sec = 0;

  endTm = startTm;
  endTm.tm_hour = 22;  // exclusive upper bound

  time_t start = mktime(&startTm);
  time_t end = mktime(&endTm);
  if (end <= start) {
    end = start + 14 * 3600;  // safety fallback
  }
  collectRangeWindow(data, start, end, result);

  if (result.count == 0) {
    // fallback to first day's 08-21 if no data matched
    struct tm fallback = localNow;
    fallback.tm_hour = 8;
    fallback.tm_min = 0;
    fallback.tm_sec = 0;
    time_t fallbackStart = mktime(&fallback);
    struct tm fallbackEndTm = fallback;
    fallbackEndTm.tm_hour = 22;
    time_t fallbackEnd = mktime(&fallbackEndTm);
    collectRangeWindow(data, fallbackStart, fallbackEnd, result);
  }
}

bool buildWindow(const WeatherData& data, const String& mode, WindowResult& result, String& message) {
  result.count = 0;
  if (mode == kModeNow) {
    collectNowWindow(data, result);
  } else if (mode == kModeToday) {
    collectTodayWindow(data, result);
  } else {
    message = "Ongeldige mode";
    return false;
  }
  if (result.count == 0) {
    message = "Onvoldoende uurdata beschikbaar.";
    return false;
  }
  return true;
}

bool computeStats(const WeatherData& data,
                  const WindowResult& window,
                  float& outMedian,
                  float& outMin,
                  float& outRain) {
  if (window.count == 0) return false;
  float temps[kMaxHourlyEntries];
  size_t tempCount = 0;
  float rainMax = 0.0f;
  bool rainValid = false;

  for (size_t i = 0; i < window.count; ++i) {
    size_t idx = window.indices[i];
    float t = data.hourlyApparent[idx];
    if (!isnan(t)) {
      temps[tempCount++] = t;
    }
    float rain = data.hourlyPrecipProb[idx];
    if (!isnan(rain)) {
      if (!rainValid || rain > rainMax) {
        rainMax = rain;
      }
      rainValid = true;
    }
  }

  if (tempCount == 0) {
    return false;
  }

  std::sort(temps, temps + tempCount);
  outMin = temps[0];
  if (tempCount % 2 == 1) {
    outMedian = temps[tempCount / 2];
  } else {
    outMedian = 0.5f * (temps[tempCount / 2 - 1] + temps[tempCount / 2]);
  }
  outRain = rainValid ? rainMax : 0.0f;
  return true;
}

String decideTop(float median, float minTemp, float rain) {
  if ((median >= 19.0f) && (minTemp >= 16.0f) && (rain < 60.0f)) {
    return "short_sleeve";
  }
  return "long_sleeve";
}

String decideBottom(float median, float minTemp, float rain) {
  if ((median >= 23.0f) && (minTemp >= 19.0f) && (rain < 50.0f)) {
    return "shorts";
  }
  return "long_pants";
}
}  // namespace

bool computeOutfitRecommendation(const WeatherData& data,
                                 const String& mode,
                                 OutfitRecommendation& out,
                                 String& message) {
  if (!data.valid || data.hourlyCount == 0) {
    message = "Geen weerdata beschikbaar.";
    return false;
  }

  String normalized = mode;
  normalized.toLowerCase();
  if (normalized != kModeNow && normalized != kModeToday) {
    message = "Mode moet 'now' of 'today' zijn.";
    return false;
  }

  WindowResult window;
  if (!buildWindow(data, normalized, window, message)) {
    return false;
  }

  float tempMedian = NAN;
  float tempMin = NAN;
  float rainMax = NAN;
  if (!computeStats(data, window, tempMedian, tempMin, rainMax)) {
    message = "Onvoldoende temperatuurdata.";
    return false;
  }

  out.valid = true;
  out.mode = normalized;
  out.tempMedian = tempMedian;
  out.tempMin = tempMin;
  out.rainProbability = rainMax;
  out.sampleCount = window.count;
  out.top = decideTop(tempMedian, tempMin, rainMax);
  out.bottom = decideBottom(tempMedian, tempMin, rainMax);
  message = "OK";
  return true;
}
