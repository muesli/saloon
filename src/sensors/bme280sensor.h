#pragma once

void setupBME280(int sda, int sclk);
void measureBME280(float* temperature, float* humidity);
