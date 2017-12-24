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
#define INITIAL_INTERVAL 1000
#define REFRESH_INTERVAL 50000
#define LED_COUNT sizeof(leds) / sizeof(Lifebox::Led)

// Main
void setup() {
  delay(10);
  Serial.begin(115200);

  testLights();

  connectToWifi();
  
  updateTimer.setTimeout(INITIAL_INTERVAL);
  updateTimer.restart();
}

void loop() {
  if (updateTimer.onRestart()) {
    updateState();
    updateNetworkStatus();
  }
  updateLights();
}

// Private
void connectToWifi() {
  WiFi.begin(LIFEBOX_WIFI_NAME, LIFEBOX_WIFI_PASS);
}

void updateNetworkStatus() {
  int wifiStatus = WiFi.status();
  int apiStatus = api.status;

  if (wifiStatus == WL_CONNECTION_LOST || apiStatus == Lifebox::Api::FAILED) {
    debugLed.off();
  }
  else if (wifiStatus == WL_CONNECTED && apiStatus == Lifebox::Api::SUCCESS) {
    debugLed.on();
  }
  else if (wifiStatus == WL_IDLE_STATUS) {
    debugLed.blink(300, 300);
  }
  else {
    debugLed.blink(100, 100);
  }
}

void updateState() {
  if (WiFi.status() == WL_CONNECTED) {
    updateTimer.setTimeout(REFRESH_INTERVAL);
    
    // Need to figure out how to make this non-blocking
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
