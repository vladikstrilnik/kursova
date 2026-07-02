#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

const char* SERVER_URL =
"http://192.168.1.100:5000/api/water";

const byte FLOW_PIN = 4;

// приклад: 1 імпульс = 1 літр
volatile uint32_t pulseCounter = 0;

float totalLiters = 0.0;
float flowRate = 0.0;

unsigned long lastMeasure = 0;

void IRAM_ATTR pulseISR()
{
    pulseCounter++;
}

void connectWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}

void sendData()
{
    if (WiFi.status() != WL_CONNECTED)
        return;

    HTTPClient http;

    http.begin(SERVER_URL);

    http.addHeader("Content-Type",
                   "application/json");

    StaticJsonDocument<256> doc;

    doc["device"] = "wc01";
    doc["liters"] = totalLiters;
    doc["flow"] = flowRate;

    String json;

    serializeJson(doc, json);

    http.POST(json);

    http.end();
}

void setup()
{
    Serial.begin(115200);

    pinMode(FLOW_PIN, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(FLOW_PIN),
        pulseISR,
        FALLING);

    connectWiFi();

    lastMeasure = millis();
}

void loop()
{
    if (millis() - lastMeasure >= 5000)
    {
        noInterrupts();

        uint32_t pulses = pulseCounter;

        pulseCounter = 0;

        interrupts();

        totalLiters += pulses;

        flowRate = pulses * 12.0;

        sendData();

        Serial.print("Water: ");
        Serial.print(totalLiters);
        Serial.print(" L   Flow: ");
        Serial.println(flowRate);

        lastMeasure = millis();
    }
}
