#include "config.h"

const uint8_t MAX_FORECASTS = 28;

#include "OpenWeatherMapCurrent.h"
#include "OpenWeatherMapForecast.h"

static OpenWeatherMapCurrentData currentWeather;
static OpenWeatherMapCurrent currentWeatherClient;

static OpenWeatherMapForecastData forecasts[MAX_FORECASTS];
static OpenWeatherMapForecast forecastClient;

void updateWeather()
{
    currentWeatherClient.setMetric(IS_METRIC);
    currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
    currentWeatherClient.updateCurrentById(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);
}

void updateWeatherForecast()
{
    forecastClient.setMetric(IS_METRIC);
    forecastClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
    uint8_t allowedHours[] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
    forecastClient.setAllowedHours(allowedHours, sizeof(allowedHours));
    forecastClient.updateForecastsById(forecasts, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID, MAX_FORECASTS);
}

OpenWeatherMapForecastData forecastForWDay(int wday)
{
    OpenWeatherMapForecastData h;
    h.weatherId = 0;

    bool first = true;
    for (int i = 0; i < MAX_FORECASTS; i++) {
        if (forecasts[i].weatherId <= 0) {
            break;
        }

        struct tm* otimeInfo;
        time_t obstime = forecasts[i].observationTime;
        otimeInfo = localtime(&obstime);
        if (otimeInfo->tm_wday != wday) {
            continue;
        }

        if (first || forecasts[i].tempMax > h.tempMax) {
            first = false;
            h = forecasts[i];
        }
    }

    return h;
}
