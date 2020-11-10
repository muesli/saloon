#include "mqtt.h"

#include <WiFiClient.h>
#include <PubSubClient.h>

#include "config.h"

#define MQTT_CLIENT_ID "saloon-%s"

static WiFiClient espClient;
static PubSubClient mqttClient(espClient);

void setupMQTT()
{
    mqttClient.setServer(MQTT_SERVER, 1883);
}

void mqttReconnect()
{
    if (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");

        char topicf[256];
        sprintf(topicf, MQTT_TOPIC_STATE, STATION_NAME);
        char clientID[256];
        sprintf(clientID, MQTT_CLIENT_ID, STATION_NAME);

        // Attempt to connect
        if (mqttClient.connect(clientID, MQTT_USER, MQTT_PASSWORD, topicf, 1, true, "disconnected", false)) {
            Serial.println("connected");

            // Once connected, publish an announcement...
            mqttClient.publish(topicf, "connected", true);
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(1000);
        }
    }
}

void mqttPublish(char *topic, float payload)
{
    char topicf[256];
    sprintf(topicf, topic, STATION_NAME);

    Serial.print(topicf);
    Serial.print(": ");
    Serial.println(payload);

    mqttClient.publish(topicf, String(payload).c_str(), true);
}

void mqttPublishState(bool state)
{
    char topicf[256];
    sprintf(topicf, MQTT_TOPIC_MOTION, STATION_NAME);

    mqttClient.publish(topicf, "motion", false);
}

void mqttLoop()
{
    if (!mqttClient.connected()) {
        mqttReconnect();
    }

    mqttClient.loop();
}
