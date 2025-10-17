#pragma once

#include "ui_auth.h"
#include "clothing_display.h"
#include "log.h"

// Initialize UI authentication and the temporary clothing display flow
// This boots the legacy display until the clothing advisor replaces it.
inline void initClothingSystem(UiAuth& uiAuth) {
    uiAuth.begin(UI_DEFAULT_PASS);
    clothingDisplaySetup();
    logInfo("🟢 Legacy clothing display initialized (pending clothing advisor)");
}
