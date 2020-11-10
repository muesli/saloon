#pragma once

#include <Arduino.h>

void setupNeoPixel(float brightness);
void setNeoPixelColor(uint16_t n, uint32_t c);
void setAllNeoPixelColor(uint r, uint g, uint b);
void rainbowCycle(uint8_t wait);
uint32_t Wheel(byte WheelPos);
