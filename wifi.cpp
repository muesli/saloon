#include "wifi.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include "config.h"
#include "notifications.h"

// #define TZ_SEC ((TIME_TZ)*3600)
// #define DST_SEC ((TIME_DST_MN)*60)

void setupWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PSK);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");

        if (NOTIFICATION_CONNECT) {
            flashLED(0, 0, 255);
        }

        delay(50);
    }

    Serial.println();
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void setupMDNS()
{
    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }
}

void updateMDNS() {
    MDNS.update();
}

void setupNTP()
{
    setenv("TZ", TIME_TZ, 1);
    configTime(0, 0, "pool.ntp.org");
}
