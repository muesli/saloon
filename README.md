# saloon

Saloon, an Arduino/ESP-based Information Monitor

## Support

### Sensors

- [x] BME280 (temperature, humidity)
- [x] DHT-11/22 (temperature, humidity)
- [x] BH1750 (light sensor)
- [x] PIR Motion Sensor
- [x] Microphone

### Displays

- [x] SH1106
- [x] SSD1306

### LEDs

- [x] Neopixel
- [x] RGB LED
- [x] Built-in LED

### APIs

- [x] MQTT
- [x] OpenWeatherMap

## Configure

Make a copy of `config.example.h` named `config.h` and start editing it to your
needs.

## Build

    arduino --pref build.path=./build --verify saloon.ino
