// Libs
#include "Led.h"
#include "Net.h"
#include "Api.h"

// Configuration
const int SLEEPING_INTERVAL = 1800000; // 30 minutes
const int LOOP_SLEEP = 25;
const size_t JSON_BUFFER = JSON_ARRAY_SIZE(7) + JSON_OBJECT_SIZE(1) + 30; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/kitchen";

// Variables
RBD::Timer updateTimer;
Lifeboxes::Led debugLed(2); // Built-in LED
Lifeboxes::Led leds[] = { 12, 14, 27, 26, 25, 33, 32 };
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
const int LED_COUNT = sizeof(leds) / sizeof(Lifeboxes::Led);


// Main
void setup() {
  Serial.begin(115200);
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
  delay(LOOP_SLEEP);
}

// Private
void updateState() {
  debugLed.on();
  
  if (net.connect()) {
    syncWithApi();
    net.disconnect();
  }
  
  debugLed.off();
}

void syncWithApi() {
  JsonObject& root = api.fetchJson();
  JsonArray& ledValues = root["led_values"];

  for (int i = 0; i < LED_COUNT; i++) {
    int state = ledValues[i];
    leds[i].setState(state);
  }
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
  debugLed.update();
}
