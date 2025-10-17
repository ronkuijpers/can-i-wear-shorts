// secrets_template.h
#pragma once

#define OTA_PASSWORD  "choose_a_strong_ota_password"
#define AP_PASSWORD "setup_portal_password"

#define VERSION_URL "version_url"

#define ADMIN_USER "ciws_admin"
#define ADMIN_PASS "change_me_admin"
#define ADMIN_REALM "Can I Wear Shorts Admin"

// Weather provider configuration (Open-Meteo does not require an API key)
#define WEATHER_PROVIDER "open-meteo"
#define WEATHER_LATITUDE "52.3702"        // decimal degrees as string
#define WEATHER_LONGITUDE "4.8952"        // decimal degrees as string
#define WEATHER_API_ENDPOINT "http://api.open-meteo.com/v1/forecast" // base URL (HTTP required)
// Example query: WEATHER_API_ENDPOINT?latitude=<lat>&longitude=<lon>&hourly=temperature_2m,precipitation&daily=temperature_2m_max,temperature_2m_min,precipitation_sum&current_weather=true&timezone=Europe/Amsterdam

// Default UI password (user is fixed as "user"). User must change on first login.
#ifndef UI_DEFAULT_PASS
#define UI_DEFAULT_PASS "changeme"
#endif
