#pragma once

void setupMQTT();
void mqttPublish(char *topic, float payload);
void mqttPublishState(bool state);
void mqttReconnect();
void mqttLoop();
