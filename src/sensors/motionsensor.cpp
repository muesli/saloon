#include "motionsensor.h"

#include <Arduino.h>

bool handleMotionSensor(int pin)
{
    int pirValue = digitalRead(pin);

    if (pirValue > 0 && !motionTriggered) {
        motionTriggered = true;
        motionLast = time(nullptr);

        /*
        Serial.println("Motion detected");
        digitalWrite(STATUS_LED_RED, LOW);
        analogWrite(STATUS_LED_BLUE, 10);
        */
        return true;
    } else if (pirValue == 0 && motionTriggered) {
        motionTriggered = false;
        motionLast = time(nullptr);

        /*
        Serial.println("Motion gone");
        analogWrite(STATUS_LED_RED, 10);
        digitalWrite(STATUS_LED_BLUE, LOW);
        */
        return true;
    }

    return false;
}
