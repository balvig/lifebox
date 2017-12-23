// Libs
#include "Led.h"
#include <WiFi.h>

// Variables
RBD::Timer timer;
Lifebox::Led leds[] = { 12, 14, 27, 26, 25, 33, 32 };
Lifebox::Led debugLed(2); // Built-in LED


// Constants
#define LED_COUNT sizeof(leds) / sizeof(Lifebox::Led)

// Main
void setup() {
  delay(10);
  Serial.begin(115200);
  timer.setTimeout(5000);
  timer.restart();
  connectToWifi();
  updateState();
}

void loop() {
  if(timer.onRestart()) {
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
  if(WiFi.status() != WL_CONNECTED) {
    debugLed.blink(300, 300);
  }
  else {
    debugLed.on();
    int results[] = { random(3), random(3) , random(3), random(2), random(2), random(2), random(2) };

    for (int i = 0; i < LED_COUNT; i++) {
      int state = results[i];
      leds[i].setState(state);
    }
  }
}

void connectToWifi() {
  WiFi.begin(".........", ".........");
}

