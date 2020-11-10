#include "api.h"

#include <ArduinoJson.h>

#include "config.h"
#include "notifications.h"

static ESP8266WebServer server(80);

void handleClient()
{
    server.handleClient();
}

void handleRoot()
{
    char msg[256];
    sprintf(msg, "Hello from saloon \"%s\"!", STATION_NAME);
    server.send(200, "text/plain", msg);
}

void handleNotification()
{
    StaticJsonDocument<256> doc;
    deserializeJson(doc, server.arg("plain"));
    String value = doc["message"];

    notify(value, 0, 255, 0);
    Serial.printf("Notification received: %s\n", value.c_str());
    server.send(200, "text/json", "{success: true}");
}

void handleUpdate()
{
    if (String(OTA_PASSWORD).length() > 0 &&
        (!server.hasArg("password") || server.arg("password") != OTA_PASSWORD)) {
        return;
    }

    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Receiving OTA Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace)) {
            // start with max available size
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
            // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
    }
}

void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setupAPI()
{
    server.onNotFound(handleNotFound);
    server.on("/", handleRoot);
    server.on("/notification", HTTP_POST, handleNotification);

    if (OTA_UPDATE_ENABLED) {
        server.on("/update", HTTP_POST, []() {
            if (String(OTA_PASSWORD).length() > 0 &&
                (!server.hasArg("password") || server.arg("password") != OTA_PASSWORD)) {
                server.send(401, "text/plain", "Password required");
                return;
            }
            server.sendHeader("Connection", "close");
            server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");

            yield();
            delay(1000);
            ESP.restart();
        }, handleUpdate);
    }

    server.begin();
    Serial.println("HTTP server started");
}
