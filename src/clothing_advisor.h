#pragma once

#include <Arduino.h>

#include "weather_client.h"

struct OutfitRecommendation {
  bool valid = false;
  String mode;
  String top;
  String bottom;
  float tempMedian = NAN;
  float tempMin = NAN;
  float rainProbability = NAN;
  size_t sampleCount = 0;
};

bool computeOutfitRecommendation(const WeatherData& data,
                                 const String& mode,
                                 OutfitRecommendation& out,
                                 String& message);
