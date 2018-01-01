// Libs
#include "Config.h"
#include "Led.h"
#include "Api.h"

#ifdef ESP_PLATFORM
  #include "WiFi.h"
#else
  #include "ESP8266WiFi.h"
#endif  

// Configuration
const int SLEEPING_INTERVAL = 1800000; // 30 minutes
const int WIFI_POLL = 600;
const int WIFI_RETRIES = 10;
const int LOOP_SLEEP = 25;
const size_t JSON_BUFFER = JSON_ARRAY_SIZE(7) + JSON_OBJECT_SIZE(1) + 30; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/kitchen";

// Variables
RBD::Timer updateTimer;
Lifeboxes::Led debugLed(2); // Built-in LED
Lifeboxes::Led leds[] = { 12, 14, 27, 26, 25, 33, 32 };
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
  if (connectToWifi(WIFI_RETRIES)) {
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
  WiFi.begin(LIFEBOXES_WIFI_NAME, LIFEBOXES_WIFI_PASS);
  
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
  debugLed.update();
}
