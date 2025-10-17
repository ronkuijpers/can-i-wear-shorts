#pragma once

#include <Adafruit_NeoPixel.h>
#include <time.h>
#include "config.h"
#include "log.h"
#include "led_controller.h"

// clockEnabled is defined externally in main.cpp
extern bool clockEnabled;

// Legacy display lifecycle (pending replacement by clothing advisor)
void clothingDisplaySetup();
void clothingDisplayLoop();

// Force the word-by-word animation to render a specific time
void clothingDisplayForceAnimationForTime(struct tm* timeinfo);


