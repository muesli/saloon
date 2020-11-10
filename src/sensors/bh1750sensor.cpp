#include "bh1750sensor.h"

#include <BH1750FVI.h>

static BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

void setupBH1750()
{
    LightSensor.begin();
}

uint16_t measureBH1750()
{
    return LightSensor.GetLightIntensity();
}
