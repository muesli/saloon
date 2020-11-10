#include "display.h"

#include "config.h"

#define U8X8_USE_PINS
#include <U8g2lib.h>

static DISPLAY_DEFINITION;

void setupDisplay()
{
    Serial.println("Setting up display...");
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_ncenB08_tr);
}

void clearDisplay()
{
    u8g2.clearBuffer();
}

void updateDisplay()
{
    u8g2.sendBuffer();
}

unsigned int displayWidth()
{
    return u8g2.getDisplayWidth();
}

unsigned int displayHeight()
{
    return u8g2.getDisplayHeight();
}

void paintString(uint x, uint y, String s)
{
    const unsigned int l = s.length() + 1;
    char b[l];
    s.toCharArray(b, l);
    u8g2.drawUTF8(x, y, b);
}

void paintCenterString(uint x, uint y, String s)
{
    const unsigned int l = s.length() + 1;
    char b[l];
    s.toCharArray(b, l);

    const int sw = u8g2.getUTF8Width(b);
    const int dw = displayWidth();

    if (sw <= dw) {
        u8g2.drawUTF8(x - sw / 2, y, b);
    } else {
        for (int i = 0; i > dw - sw; i--) {
            u8g2.drawUTF8(i, y, b);
            updateDisplay();
            delay(1000 / 100);
        }
        for (int i = dw - sw; i < 0; i++) {
            u8g2.drawUTF8(i, y, b);
            updateDisplay();
            delay(1000 / 100);
        }
    }
}

void paintWeatherIconAndTemp(uint x, uint y, float temperature, int weatherId, tm* timeInfo)
{
    u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);
    unsigned int glyph = 68;
    if (weatherId >= 200) {
        glyph = 67;
    }
    if (weatherId >= 300) {
        u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
        glyph = 67;
    }
    if (weatherId >= 800) {
        if (timeInfo->tm_hour <= 6 || timeInfo->tm_hour >= 21) {
            glyph = 66;
        } else {
            glyph = 69;

            if (weatherId >= 801) {
                glyph = 65;
            }
            if (weatherId >= 803) {
                glyph = 64;
            }
        }
    }
    u8g2.drawGlyph(x, y + 38, glyph);

    u8g2.setFont(u8g2_font_logisoso24_tf);
    paintString(x + 40, y + 36, String(temperature, 1) + "°");
}

void paintWeatherDesc(uint x, uint y, String desc)
{
    u8g2.setFont(u8g2_font_8x13_mf);
    paintCenterString(u8g2.getDisplayWidth() / 2, u8g2.getDisplayHeight() - 6, desc);
}

void paintTime(uint x, uint y, tm* timeInfo)
{
    char buf[64];
    sprintf_P(buf, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(u8g2.getDisplayWidth() / 2 - u8g2.getStrWidth(buf) / 2, u8g2.getDisplayHeight() - 6, buf);
}
