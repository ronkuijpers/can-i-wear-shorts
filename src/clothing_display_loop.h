#pragma once

#include "clothing_display.h"
#include "log.h"

// Legacy display logic (ex-Wordclock) to be replaced by the clothing advisor loop
// Called periodically from the main loop while the refactor is in progress.
inline void runClothingDisplayLoop() {
    clothingDisplayLoop();
}
