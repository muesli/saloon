#include "microphone.h"

#include <Arduino.h>

// Sample window width in mS (50 mS = 20Hz)
const int sampleWindow = 50;

double measureNoiseLevel(int pin)
{
    unsigned int sample;
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;
    unsigned long startMillis = millis();

    while (millis() - startMillis < sampleWindow) {
        sample = analogRead(pin);
        if (sample < 1024) {
            // toss out spurious readings
            if (sample > signalMax) {
                // save just the max levels
                signalMax = sample;
            }
            if (sample < signalMin) {
                // save just the min levels
                signalMin = sample;
            }
        }
    }

    if (signalMin >= signalMax) {
        return 0;
    }

    // convert to volts
    unsigned int peakToPeak = signalMax - signalMin;
    return (peakToPeak * 5.0) / 1024;
}
