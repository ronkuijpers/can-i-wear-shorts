# Can I Wear Shorts – Quick Start

## 1. Product Overview
- ESP32-gestuurde LED-matrix die toont welk type kleding (shorts, lange broek, regenjas, etc.) vandaag geschikt is.
- Werkt binnenshuis met een 5V-voeding en 161 NeoPixels (zelfde hardware-opstelling als de oorspronkelijke Wordclock).

## 2. Box Contents
- Can I Wear Shorts-unit
- 5V power adapter
- Printed instructions (this sheet)

## 3. Initial Power-Up
1. Plug in the power adapter.
2. Wait for the device to boot (about 10 seconds). De LED-matrix toont een korte startanimatie.

## 4. Connect to WiFi
1. On first power-up (or after a WiFi reset) the device creates a temporary WiFi network named **`CanIWearShorts_AP`** with the password printed on your device or configured in `include/secrets.h`.
2. Connect with your phone or computer and a setup page opens automatically. If not, visit `http://192.168.4.1`.
3. Select your home WiFi, enter the password, and save. The device will reboot and join your network.

## 5. Open the Dashboard
1. After joining WiFi, find the device’s IP address in your router or use `http://can-i-wear-shorts.local` (mDNS) in most browsers.
2. The dashboard lets you:
   - Toggle the LED-output on/off
   - View the log output
   - Adjust brightness and LED color
   - Start the startup/showcase sequence
   - Restart or reset WiFi
   - Check for firmware updates or upload a `.bin` file
   - (Soon) Inspect weather data and override clothing thresholds

## 6. Time Synchronization
De ESP32 synchroniseert de tijd via NTP om vaste update-momenten (voor weerdata en automatische firmware-checks) aan te houden.

## 7. Firmware Updates
- Each night at 02:00 the device checks for new firmware and installs it automatically if found.
- You can also trigger an update manually from the dashboard using **Check for updates** or by uploading a firmware file.

## 8. Resetting WiFi / Restarting
- Use the dashboard buttons **Reset WiFi** or **Restart Can I Wear Shorts** when moving the device or troubleshooting.
- After a WiFi reset the setup access point reappears.

## 9. Troubleshooting & Support
If the device won't connect or shows unexpected LED output:
- Verify your WiFi credentials.
- Restart via the dashboard or by briefly disconnecting power.
- Check the log on the dashboard for error messages.
- For additional help visit the project repository.

**Firmware version:** 0.9 (placeholder – update via `firmware.json` when publishing)
