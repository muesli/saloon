#pragma once

#include <Arduino.h>
#include <time.h>

const String WDAY_NAMES[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void setupDisplay();
void clearDisplay();
void updateDisplay();

unsigned int displayWidth();
unsigned int displayHeight();

void paintCenterString(uint x, uint y, String s);

void paintWeatherIconAndTemp(uint x, uint y, float temperature, int weatherId, tm* timeInfo);
void paintWeatherDesc(uint x, uint y, String desc);
void paintTime(uint x, uint y, tm* timeInfo);
