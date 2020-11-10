#include "api.h"
#include "config.h"
#include "display.h"
#include "mqtt.h"
#include "neopixel.h"
#include "notifications.h"
#include "wifi.h"

#include "src/sensors/bh1750sensor.h"
#include "src/sensors/bme280sensor.h"
#include "src/sensors/dhtsensor.h"
#include "src/sensors/microphone.h"
#include "src/sensors/motionsensor.h"

#include "src/weather/openweather.h"

const String SALOON_VER = "0.0.1";

long lastWeatherUpdate = 0;
long lastForecastUpdate = 0;
long lastMQTTPublish = 0;
long lastSensorUpdate = 0;
long lastUpdate = 0;

static float temperature = 0;
static float humidity = 0;
static double noise = 0;
static uint16_t lux = 0;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.printf("\Saloon v%s\n", SALOON_VER.c_str());

    setupPins();

    if (NEOPIXEL >= 0) {
        setupNeoPixel(((NEOPIXEL_BRIGHTNESS) / 100.0) * 255);
    }

    setupDisplay();
    paintCenterString(displayWidth() / 2, displayHeight() / 2, "Connecting...");
    updateDisplay();

    setupWifi();
    setupNTP();
    setupMDNS();
    setupAPI();
    setupMQTT();

    if (DHT_SENSOR >= 0) {
        setupDHT(DHT_SENSOR, DHT_SENSOR_TYPE);
    }
    if (BME280_SENSOR_CLK >= 0) {
        setupBME280(BME280_SENSOR_SDA, BME280_SENSOR_CLK);
    }
    if (BH1750_SENSOR_CLK >= 0) {
        setupBH1750();
    }

    clearDisplay();
    paintCenterString(displayWidth() / 2, displayHeight() / 2, "Gathering data...");
    updateDisplay();
}

void setupPins()
{
    pinMode(MOTION_PIR_SENSOR, INPUT);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    digitalWrite(STATUS_LED, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
}

void loop()
{
    time_t now = time(nullptr);
    struct tm timeInfo;
    timeInfo = *(localtime(&now));

    cron(timeInfo);
    handleClient();
    updateMDNS();
    mqttLoop();

    /* Motion Sensor */
    bool motionChange = handleMotionSensor(MOTION_PIR_SENSOR);
    if (motionChange) {
        mqttPublishState(true);

        if (NOTIFICATION_MOTION) {
            notify("Motion triggered!", 255, 255, 0);
        }
    }

    /* Update Display */
    clearDisplay();

    OpenWeatherMapForecastData forecast;
    if (timeInfo.tm_hour < 12) {
        forecast = forecastForWDay(timeInfo.tm_wday);
    } else {
        forecast = forecastForWDay((timeInfo.tm_wday + 1) % 7);
    }

    int smod = timeInfo.tm_sec / 5;
    int step = smod % 4;
    if (step == 2 && forecast.weatherId <= 0) step = 0;
    if (step < 2 && forecast.weatherId <= 0) step = 3;

    switch (step) {
        case 0:
        {
            paintWeatherIconAndTemp(16, 0, currentWeather.temp, currentWeather.weatherId, &timeInfo);
            paintTime(0, 0, &timeInfo);
        }
        break;

        case 1:
        {
            paintWeatherIconAndTemp(16, 0, currentWeather.temp, currentWeather.weatherId, &timeInfo);
            paintWeatherDesc(0, 0, currentWeather.description);
        }
        break;

        case 2:
        {
            struct tm* ftimeInfo;
            time_t obstime = forecast.observationTime;
            ftimeInfo = localtime(&obstime);

            paintWeatherIconAndTemp(16, 0, forecast.tempMax, forecast.weatherId, ftimeInfo);
            paintWeatherDesc(0, 0, WDAY_NAMES[ftimeInfo->tm_wday] + ", " + String(ftimeInfo->tm_hour) + "h");
        }
        break;

        case 3:
        {
            paintWeatherIconAndTemp(16, 0, temperature, 0, &timeInfo);
            paintWeatherDesc(0, 0, "Indoor");
        }
        break;
    }

    updateDisplay();
}

void cron(tm timeInfo)
{
    if (timeInfo.tm_min != lastUpdate) {
        lastUpdate = timeInfo.tm_min;
        if (timeInfo.tm_min == 0) {
            if (NOTIFICATION_HOUR) {
                // flash once per full hour of the day, at the full hour
                int h = timeInfo.tm_hour;
                if (h == 0) {
                    h = 12;
                }
                if (h > 12) {
                    h -= 12;
                }
                for (int x = 0; x < h; x++) {
                    flashLED(255, 255, 255);
                }
            }
        } else {
            // flash once per minute
            if (NOTIFICATION_MINUTE) {
                flashLED(64, 64, 64);
            }
        }
    }

    /* Update sensor values */
    if (lastSensorUpdate == 0 || millis() - lastSensorUpdate > (1000L * (UPDATE_SENSOR_INTERVAL_SECS))) {
        updateSensors();
        lastSensorUpdate = millis();
    }

    /* Publish MQTT values */
    if (lastMQTTPublish == 0 || millis() - lastMQTTPublish > (1000L * (PUBLISH_MQTT_INTERVAL_SECS))) {
        publishMQTTValues();
        lastMQTTPublish = millis();
    }

    /* Update Weather & Forecast */
    if (lastWeatherUpdate == 0 || millis() - lastWeatherUpdate > (1000L * (UPDATE_WEATHER_INTERVAL_SECS))) {
        Serial.println("Getting weather...");
        updateWeather();
        lastWeatherUpdate = millis();
    }
    if (lastForecastUpdate == 0 || millis() - lastForecastUpdate > (1000L * (UPDATE_FORECAST_INTERVAL_SECS))) {
        Serial.println("Getting forecast...");
        updateWeatherForecast();
        lastForecastUpdate = millis();
    }
}

void updateSensors()
{
    if (DHT_SENSOR >= 0) {
        measureDHT(&temperature, &humidity);
    }
    if (BME280_SENSOR_CLK >= 0) {
        measureBME280(&temperature, &humidity);
    }
    if (MICROPHONE >= 0) {
        noise = measureNoiseLevel(MICROPHONE);
    }
    if (BH1750_SENSOR_CLK >= 0) {
        lux = measureBH1750();
    }
}

void publishMQTTValues()
{
    /* Weather */
    if (MQTT_PUBLISH_WEATHER && (currentWeather.temp != 0 || currentWeather.humidity > 0)) {
        mqttPublish(MQTT_TOPIC_OUTSIDE_TEMPERATURE, currentWeather.temp);
        mqttPublish(MQTT_TOPIC_OUTSIDE_HUMIDITY, currentWeather.humidity);
    }

    /* Temperature */
    if (temperature != 0 || humidity > 0) {
        mqttPublish(MQTT_TOPIC_TEMPERATURE, temperature);
        mqttPublish(MQTT_TOPIC_HUMIDITY, humidity);
    }

    /* Microphone */
    if (MICROPHONE >= 0) {
        mqttPublish(MQTT_TOPIC_NOISELEVEL, noise);
    }

    /* Light Intensity */
    if (BH1750_SENSOR_CLK >= 0) {
        mqttPublish(MQTT_TOPIC_LIGHTINTENSITY, lux);
    }
}
