#pragma once

// General Settings

/**
 * The station's name
 */
#define STATION_NAME "livingroom"

/**
 * Your local time-zone offset from UTC+ in hours
 * Look up your definition at:
 *   https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
 */
#define TIME_TZ "CET-1CEST,M3.5.0,M10.5.0/3"

/**
 * Use metric units
 */
#define IS_METRIC true

// Intervals
#define UPDATE_WEATHER_INTERVAL_SECS 15 * 60 // Update every 15 minutes
#define UPDATE_FORECAST_INTERVAL_SECS 60 * 60 // Update every hour
#define UPDATE_SENSOR_INTERVAL_SECS 60 // Update every minute
#define PUBLISH_MQTT_INTERVAL_SECS 60 // Update every minute

// Wifi Configuration
#define WIFI_SSID "ssid"
#define WIFI_PSK "password"

// OTA Update Settings

/**
 * Enables remote firmware updates via HTTP
 */
#define OTA_UPDATE_ENABLED true
#define OTA_PASSWORD "admin"

// MQTT Configuration
#define MQTT_SERVER "192.168.0.1"
#define MQTT_USER "saloon"
#define MQTT_PASSWORD "saloon"

// MQTT topic definitions. "%s" will get replaced by the STATION_NAME
#define MQTT_TOPIC_OUTSIDE_TEMPERATURE "home/outside/temperature"
#define MQTT_TOPIC_OUTSIDE_HUMIDITY "home/outside/humidity"
#define MQTT_TOPIC_HUMIDITY "home/%s/humidity"
#define MQTT_TOPIC_TEMPERATURE "home/%s/temperature"
#define MQTT_TOPIC_STATE "home/%s/status"
#define MQTT_TOPIC_MOTION "home/%s/motion"
#define MQTT_TOPIC_LIGHTINTENSITY "home/%s/lightintensity"
#define MQTT_TOPIC_NOISELEVEL "home/%s/noiselevel"

/**
 * Publishes the retrieved OpenWeather data to MQTT when true
 */
#define MQTT_PUBLISH_WEATHER true

// Display Configuration
#define DISPLAY_DEFINITION U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE)
// #define DISPLAY_DEFINITION U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE)
// #define DISPLAY_DEFINITION U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, D1, D2, U8X8_PIN_NONE)

// Pin Configuration

/**
 * Pin the PIR motion sensor is connected to
 *
 * Set to -1 to disable the sensor
 */
#define MOTION_PIR_SENSOR D0

/**
 * Pin the BME280 air sensor CLK & SLA signals are connected to
 *
 * Set to -1 to disable the sensor
 */
#define BME280_SENSOR_CLK D1
#define BME280_SENSOR_SDA D2

/**
 * Pin the DHT air sensor is connected to
 *
 * Set to -1 to disable the sensor
 */
#define DHT_SENSOR -1 // D8

/**
 * Which type of DHT sensor to use
 *
 *  0 for auto-detection
 * 11 for DHT-11
 * 22 for DHT-22
 */
#define DHT_SENSOR_TYPE 0

/**
 * Pin the BH1750 light sensor CLK is connected to
 *
 * Set to -1 to disable the sensor
 */
#define BH1750_SENSOR_CLK D1 // D1

/**
 * Pin the microphone is connected to
 *
 * Set to -1 to disable the sensor
 */
#define MICROPHONE A0 // A0

/**
 * Pin of the status LED
 *
 * Set to -1 to disable the status LED
 */
#define STATUS_LED LED_BUILTIN

/**
 * Notification Settings
 */
#define NOTIFICATION_CONNECT true
#define NOTIFICATION_HOUR true
#define NOTIFICATION_MINUTE false
#define NOTIFICATION_MOTION false

/**
 * Pins of the notification LEDs
 *
 * Set to -1 to disable the notification LEDs
 */
#define LED_BLUE D5
#define LED_GREEN D6
#define LED_RED D8

/**
 * Neopixel notification LED configuration
 */
#define NEOPIXEL -1 // D7
#define NEOPIXEL_PIXEL_COUNT 12
#define NEOPIXEL_BRIGHTNESS 5 // in percent

// OpenWeatherMap Settings

/**
 * Your OpenWeatherMap API key
 *
 * Sign up here to get an API key: https://openweathermap.org/api
 */
#define OPEN_WEATHER_MAP_APP_ID "your_api_key"

/**
 * Location ID for weather forecasts
 *
 * Go to https://openweathermap.org/find?q= and search for a location. Go through the
 * result set and select the entry closest to the actual location you want to display
 * data for. It'll be a URL like https://openweathermap.org/city/2954172. The number
 * at the end is what you assign to the constant below.
 */
#define OPEN_WEATHER_MAP_LOCATION_ID "1234"

/**
 * Language for weather forecasts
 *
 * Pick a language code from this list:
 *
 * Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el,
 * English - en, Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl,
 * Croatian - hr, Hungarian - hu, Italian - it, Japanese - ja, Korean - kr,
 * Latvian - la, Lithuanian - lt, Macedonian - mk, Dutch - nl, Polish - pl,
 * Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
 * Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi,
 * Chinese Simplified - zh_cn, Chinese Traditional - zh_tw.
 */
#define OPEN_WEATHER_MAP_LANGUAGE "en"
