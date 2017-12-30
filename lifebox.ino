// Libs
#include "Config.h"
#include "Led.h"
#include "Api.h"
#include <WiFi.h>

// Variables
RBD::Timer updateTimer;
Lifebox::Led debugLed(2); // Built-in LED
Lifebox::Led leds[] = { 12, 14, 27, 26, 25, 33, 32 };
Lifebox::Api api("http://starter-api-production.herokuapp.com/lifebox");

// Constants
#define SLEEPING_INTERVAL 900000 // 15 minutes
//#define SLEEPING_INTERVAL 10000 // 15 minutes
#define WIFI_POLL 600
#define LED_COUNT sizeof(leds) / sizeof(Lifebox::Led)

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
  String results = api.fetch();

  // All this to turn csv into an array
  char charBuf[50];
  results.toCharArray(charBuf, 50);
  char *token = strtok(charBuf, ",");
  int i = 0;

  while (token) {
    String tokenString = token;
    int state = tokenString.toInt();
    leds[i].setState(state);
    token = strtok(NULL, ",");
    i++;
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
