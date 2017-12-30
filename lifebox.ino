// Libs
#include "Config.h"
#include "Led.h"
#include "Api.h"
#include <WiFi.h>

// Configuration
const int SLEEPING_INTERVAL = 900000; // 15 minutes
const int WIFI_POLL = 600;
const size_t JSON_BUFFER = JSON_ARRAY_SIZE(7) + JSON_OBJECT_SIZE(1) + 30;
const String API_ENDPOINT = "http://starter-api-production.herokuapp.com/lifebox";

// Variables
RBD::Timer updateTimer;
Lifebox::Led debugLed(2); // Built-in LED
Lifebox::Led leds[] = { 12, 14, 27, 26, 25, 33, 32 };
const int LED_COUNT = sizeof(leds) / sizeof(Lifebox::Led);
Lifebox::Api api(API_ENDPOINT, JSON_BUFFER);


// Main
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  testLights();
  updateTimer.setTimeout(SLEEPING_INTERVAL);
  updateTimer.restart();
  updateState();
}

void loop() {
  if (updateTimer.onRestart()) {
    updateState();
  }
  updateLights();
}

// Private
void updateState() {
  if (connectToWifi(10)) {
    syncWithApi();
    disconnectFromWifi();
  }
}


void syncWithApi() {
  JsonObject& root = api.fetchJson();
  JsonArray& ledValues = root["led_values"];

  for (int i = 0; i < LED_COUNT; i++) {
    int state = ledValues[i];
    leds[i].setState(state);
  }
}

bool connectToWifi(int tries) {
  WiFi.begin(LIFEBOX_WIFI_NAME, LIFEBOX_WIFI_PASS);
  
  for(int i = 0; i < tries; i++) {
    int wifiStatus = WiFi.status();
    debugLed.on();
    delay(WIFI_POLL); // blocking all the things
    debugLed.off();
    delay(WIFI_POLL); // blocking all the things

    Serial.println(wifiStatus);
    if (wifiStatus == WL_CONNECTED) {
      return true;
    }
  }

  debugLed.pulse();
  return false;
}

void disconnectFromWifi() {
  WiFi.disconnect();
}

void testLights() {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i].on();
  }
  delay(1000);
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i].off();
  }
}

void updateLights() {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i].update();
  }
}
