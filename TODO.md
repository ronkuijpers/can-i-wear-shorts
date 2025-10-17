# TODO voor "Can I Wear Shorts"

## Branding & Configuratie
- [x] Hernoem projectnaam, hostnamen en WiFi/AP-identifiers (bijv. `CLOCK_NAME`, `AP_NAME`, mDNS) van Wordclock naar Can-I-Wear-Shorts in `src/config.h`, `include/secrets_template.h` en `firmware.json`.
- [x] Pas documentatie (`README.md`, `docs/BuildInstruction.md`, `docs/QuickStart.md`) aan zodat het nieuwe doel – kledingadvies via LED-display op basis van weersverwachting – wordt beschreven.
- [x] Controleer `platformio.ini`, `upload_params_template.ini` en scripts in `tools/` op Wordclock-specifieke namen of paden en werk deze bij (geen updates nodig).

## Weerdata & Advieslogica
- [x] Kies een weerbron (Open-Meteo; geen API key nodig) en documenteer benodigde locatie/tijdzoneparameters in `include/secrets_template.h`.
- [ ] Voorzie een configuratiestap waarbij de gebruiker eerst locatie (lat/lon) en tijdzone vastlegt voordat de Open-Meteo API wordt aangeroepen.
- [ ] Implementeer een `weather_client` module die periodiek de weersverwachting ophaalt (HTTP(S) + JSON), inclusief caching, retry en timeouts.
- [ ] Ontwerp een `clothing_advisor` component die temperatuur, gevoelstemperatuur, wind, neerslag en zon combineert tot kledingcategorieën (shorts, lange broek, regenjas, jas + laagjes, etc.).
- [ ] Vervang de legacy displaylaag (`clothing_display.cpp`, `clothing_display.h`, `clothing_display_loop.h`) en `time_mapper.*` door logica die het kledingadvies berekent en vertaalt naar LED-segmenten/animaties.
- [ ] Verwijder Wordclock-specifieke instellingen (bijv. "sell mode", HET/IS animaties, grid-varianten) en introduceer configuratie voor adviesdrempels en weermetingen.
- [ ] Voeg unit tests toe onder `test/` voor de advieslogica met scenario's voor verschillende weersituaties.

## LED-weergave & Animaties
- [ ] Definieer een nieuwe LED-layout of iconenset (vervang `src/grid_layout.*`, `src/wordposition.h`, gerelateerde helpers) die kledingopties of kleuren representeren.
- [ ] Pas `led_state` en `display_settings` aan zodat helderheid/kleuren behoud blijven maar Wordclock-gridreferenties worden verwijderd.
- [ ] Ontwerp een nieuwe opstartsequentie (`src/sequence_controller.h`, `src/startup_sequence_init.h`) die bij het kledingadvies past, bijvoorbeeld een korte showcase van de mogelijke outfits.
- [ ] Werk `initDisplay()` bij in `src/display_init.h` zodat het nieuwe layout- en instellingenmodel wordt geïnitialiseerd.

## Webinterface & API
- [ ] Verwijder de Wordclock-specifieke toggles voor woord-animatie en HET-IS-duur uit de dashboard UI.
- [x] Voeg invoervelden toe aan de dashboard UI waarmee een gebruiker een locatie (stad/adres) kan opslaan; toon de gekozen locatie (lat/lon + tijdzone) direct in de interface.
- [ ] Herontwerp de webbestanden in `data/` zodat het dashboard de huidige weersverwachting en kledingaanbeveling toont, inclusief configuratie van drempels.
- [ ] Pas REST-endpoints in `src/web_routes.h` aan naar API's voor weerstatus, kledingadvies, configuratie en handmatige refresh.
- [ ] Update tekststrings voor authenticatie/portaal (bv. "Wordclock UI") naar de nieuwe projectnaam.
- [ ] Controleer OTA/UI-updateflows (`src/ota_updater.cpp`, `firmware.json`) en verwijs naar de nieuwe repository/versies.

## Integraties & Services
- [ ] Beslis of MQTT-integratie behouden blijft; zo ja, herstructureer `src/mqtt_client.*` en `src/mqtt_settings.*` naar topics rond kledingadvies (bijv. `can_i_wear_shorts/advice`).
- [ ] Valideer tijdsynchronisatie (`src/time_sync.*`) voor het plannen van weerupdates en voeg fallbacks toe wanneer NTP niet beschikbaar is.
- [ ] Test OTA (`src/ota_init.h`, `src/ota_updater.cpp`) met de nieuwe firmwarelocaties.

## Templates & Documentatie
- [ ] Documenteer in `include/secrets_template.h`/`docs/QuickStart.md` dat `WEATHER_LATITUDE` en `WEATHER_LONGITUDE` door de gebruiker ingevuld moeten worden voordat weerupdates werken.
- [ ] Voeg in de UI/dashboard een setup wizard toe die via `https://nominatim.openstreetmap.org/search?q=<stad>&format=json` locatie zoekt en de tijdzone door de gebruiker laat kiezen (geen automatische fetch).
- [ ] Actualiseer `docs/todo` en andere referentiedocumenten zodat ze de nieuwe backlog weerspiegelen.
- [ ] Review en update hulpmiddelen in `tools/` (bijv. upload scripts) die mogelijk van Wordclock-uitgangspunten uitgaan.
