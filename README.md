# Can I Wear Shorts – Firmware

Firmware for an ESP32-based LED display that advises what kind of clothing to wear today, driven by the local weather forecast. The project started life as the Wordclock firmware and is currently being refactored towards weather-aware clothing guidance.

## Status

> Development in progress.  
> Functionality is incomplete and unstable.  
> Use at your own risk.

## Features (current & planned)

- WiFi setup via WiFiManager
- OTA firmware updates
- Web-based dashboard and admin interface
- Telnet-style logging over serial
- NeoPixel LED matrix control
- Weather ingestion and clothing recommendation pipeline (WIP)

## Installation & Hardware

See [docs/BuildInstruction.md](docs/BuildInstruction.md) for hardware connections and LED configuration.

Requirements:
- ESP32 microcontroller
- NeoPixel LED strip/matrix
- Power supply (VIN + GND)

## Configuration

1. Copy `include/secrets_template.h` to `include/secrets.h` and fill in your WiFi credentials.
2. Modify `upload_params_template.ini` if necessary and rename it to `upload_params.ini`.
3. Compile and upload the firmware using PlatformIO.

## Usage

After installation, the device is accessible via the local network. Use the web dashboard for settings, updates and (soon) weather-driven advice.

## Project Structure & Modules

The repository still mirrors the original Wordclock layout; the ongoing refactor will gradually replace clock-specific modules with weather and clothing advice logic. Key entry points today:

- `main.cpp`: Core setup/loop that wires together networking, OTA, web UI and LED control.
- `network_init.h`, `ota_init.h`, `time_sync.h`: Connectivity helpers.
- `webserver_init.h`, `web_routes.h`: Embedded web dashboard and REST API (subject to redesign).
- `mqtt_init.h`, `mqtt_client.*`: Optional MQTT integration, to be re-targeted for clothing advice data.
- `display_init.h`, `led_controller.*`, `led_state.*`: LED hardware abstraction.
- Legacy display modules (`clothing_display.*`, `grid_layout.*`, `time_mapper.*`) – slated for replacement by weather/clothing components.

## Documentation

- [BuildInstruction.md](docs/BuildInstruction.md): Hardware and LED setup
- [QuickStart.md](docs/QuickStart.md): Quick Start Guide
- [todo](docs/todo): Pending and completed tasks

## Roadmap

- Replace the legacy text-mapping logic with weather ingestion (`weather_client`) and decision engine (`clothing_advisor`).
- Introduce new LED layout and animations tailored to clothing categories.
- Refresh the web dashboard with weather status, thresholds and manual override controls.
- Update OTA feeds and documentation once the new behaviour is stable.

## License

To be determined.
