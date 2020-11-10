#include "bme280sensor.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

static Adafruit_BME280 bme; // I2C

void setupBME280(int sda, int sclk)
{
    Wire.begin(sda, sclk);
    Wire.setClock(100000);

    if (!bme.begin(0x76)) {
        Serial.println("Could not initialize BME280 sensor!");
    }
}

void measureBME280(float* temperature, float* humidity)
{
    static float t, h;

    t = bme.readTemperature();
    h = bme.readHumidity();

    *temperature = t;
    *humidity = h;
}
