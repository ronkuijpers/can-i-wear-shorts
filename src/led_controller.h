#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Adafruit_NeoPixel.h>
#include <vector>

// Export the function prototypes:
void initLeds();
void showLeds(const std::vector<uint16_t> &ledIndices);

#endif // LED_CONTROLLER_H
