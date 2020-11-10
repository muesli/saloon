#include "dhtsensor.h"

#include <Arduino.h>
#include "DHTesp.h"

static DHTesp dht;

void setupDHT(int pin, int sensor_type)
{
    if (sensor_type == 11) {
        dht.setup(pin, DHTesp::DHT11);
    } else if (sensor_type == 22) {
        dht.setup(pin, DHTesp::DHT22);
    }
}

void measureDHT(float* temperature, float* humidity)
{
    static unsigned long measurement_timestamp = millis();
    static float t, h;

    /* Measure once every four seconds. */
    if (millis() - measurement_timestamp > dht.getMinimumSamplingPeriod() * 4) {
        float nt, nh;
        nt = dht.getTemperature();
        nh = dht.getHumidity();

        if (nt == nt) { // check for nt being NaN
            t = nt;
        }
        if (nh == nh) { // check for nh being NaN
            h = nh;
        }

        measurement_timestamp = millis();
    }

    *temperature = t;
    *humidity = h;
}
