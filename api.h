#pragma once

#include <ESP8266WebServer.h>

void handleClient();
void handleRoot();
void handleNotification();
void handleNotFound();
void setupAPI();
