#pragma once

#include <time.h>

static bool motionTriggered = false;
static time_t motionLast = time(nullptr);

bool handleMotionSensor(int pin);
