// Libs
#include "Config.h"
#include "Led.h"
#include "Api.h"
#include <WiFi.h>

// Variables
RBD::Timer timer;
Lifebox::Led leds[] = { 12, 14, 27, 26, 25, 33, 32 };
Lifebox::Led debugLed(2); // Built-in LED
Lifebox::Api api("http://1b524aa3.ngrok.io/lifebox");

// Constants
#define REFRESH_INTERVAL 50000
#define LED_COUNT sizeof(leds) / sizeof(Lifebox::Led)

// Main
void setup() {
  delay(10);
  Serial.begin(115200);
  timer.setTimeout(REFRESH_INTERVAL);
  timer.restart();
  
  connectToWifi();
  updateState();
}

void loop() {
  if (timer.onRestart()) {
    updateState();
  }
  updateLights();
}

// Private
void updateLights() {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i].update();
  }
  debugLed.update();
}

void updateState() {
  debugLed.blink(300, 300);

  if (WiFi.status() == WL_CONNECTED) {
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

    debugLed.on();
  }
}

void connectToWifi() {
  WiFi.begin(LIFEBOX_WIFI_NAME, LIFEBOX_WIFI_PASS);
}
