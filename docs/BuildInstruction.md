# Can I Wear Shorts – Hardware & Opbouw

De hardware is gelijk aan de oorspronkelijke Wordclock: een ESP32 met een NeoPixel-strip of -matrix. Onderstaande stappen helpen je de LED’s en voeding correct aan te sluiten.

## Voeding en Data
- Sluit 5V aan op **VIN** en **GND** voor voeding.
- Verbind de datalijn van de LED-strip met **GPIO D4** van de ESP32.

## LED-strip plaatsen
- Begin met plakken in de **rechterbovenhoek** van de matrix.
- **Eerste LED overslaan** (index 0 blijft vrij als service-/statuspunt).
- Bij iedere hoek **4 LED’s overslaan** om genoeg kabelruimte te houden voor een nette bocht.

## Toekomstige aanpassingen
Zodra de kledingadvies-layout definitief is, komt hier een bijgewerkt schema met posities voor iconen/segmenten in plaats van Wordclock-woorden.

