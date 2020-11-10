#include "notifications.h"

#include "config.h"
#include "display.h"
#include "neopixel.h"

void setColor(uint r, uint g, uint b)
{
    if (LED_RED >= 0 && LED_GREEN >= 0 && LED_BLUE >= 0) {
        analogWrite(LED_RED, r);
        analogWrite(LED_GREEN, g);
        analogWrite(LED_BLUE, b);
    }
    if (NEOPIXEL >= 0) {
        setAllNeoPixelColor(r, g, b);
    }
}

void flashLED(uint r, uint g, uint b)
{
    if (NEOPIXEL >= 0) {
        rainbowCycle(3);
        setColor(0, 0, 0);
        return;
    }

    for (int i = 0; i < 255; i++) {
        setColor((float(i) / 255.0) * float(r),
            (float(i) / 255.0) * float(g),
            (float(i) / 255.0) * float(b));
        delay(1);
    }
    delay(50);

    for (int i = 0; i < 255; i++) {
        setColor((1.0 - (float(i) / 255.0)) * float(r),
            (1.0 - (float(i) / 255.0)) * float(g),
            (1.0 - (float(i) / 255.0)) * float(b));
        delay(1);
    }
    setColor(0, 0, 0);
}

void notify(String text, uint r, uint g, uint b)
{
    clearDisplay();
    paintCenterString(displayWidth() / 2, displayHeight() / 2, text);
    updateDisplay();

    for (int x = 0; x < 5; x++) {
        flashLED(r, g, b);
    }
}
