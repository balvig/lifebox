// Libs
#include "Led.h"

// Variables
RBD::Timer timer;
Lifebox::Led leds[] = { 12, 14, 27, 26, 25, 33, 32 };


// Constants
#define LED_COUNT sizeof(leds) / sizeof(Lifebox::Led)

// Main
void setup() {
  timer.setTimeout(5000);
  timer.restart();
  updateState();
  Serial.begin(115200);
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
}

void updateState() {
  int results[] = { random(3), random(3) , random(3), random(2), random(2), random(2), random(2) };

  for (int i = 0; i < LED_COUNT; i++) {
    int state = results[i];
    leds[i].setState(state);
  }
}


