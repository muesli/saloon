#include "neopixel.h"

#include <Adafruit_NeoPixel.h>

#include "config.h"

static Adafruit_NeoPixel np = Adafruit_NeoPixel(NEOPIXEL_PIXEL_COUNT, NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setupNeoPixel(float brightness)
{
    np.begin();
    np.setBrightness(brightness);
}

void setNeoPixelColor(uint16_t n, uint32_t c)
{
    np.setPixelColor(n, c);
}

void setAllNeoPixelColor(uint r, uint g, uint b)
{
    auto col = np.Color(r, g, b);
    for (uint16_t i = 0; i < np.numPixels(); i++) {
        setNeoPixelColor(i, col);
    }
    np.show();
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for(j=0; j<256*2; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< np.numPixels(); i++) {
            setNeoPixelColor(i, Wheel(((i * 256 / np.numPixels()) + j) & 255));
        }
        np.show();
        delay(wait);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return np.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return np.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return np.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
