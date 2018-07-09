// Libs
#include "Api.h"
#include "ConfigurableNet.h"
#include "Sleep.h"
#include "Servo.h"

// Configuration
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 20; // http://arduinojson.org/assistant/
//const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/recycle";
const String API_ENDPOINT = "http://61c38b52.ngrok.io";
const int INIT_HAND_POSITION = 180;
const int HAND_PIN = 2;

// Variables
Lifeboxes::ConfigurableNet net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Sleep sleep(1);
Servo hand;


// Main
void setup() {
  if(sleep.isTimeToWakeUp()) {
    syncWithApi();
  }

  sleep.goToSleep();
}

void loop() {
}

void syncWithApi() {
  net.setErrorCallback(wifiError);
  net.connect();
  JsonObject& root = api.fetchJson();
  int degrees = root["degrees"] | INIT_HAND_POSITION;
  setHand(degrees);
}

void setHand(int degrees) {
  hand.attach(HAND_PIN);
  hand.write(degrees);
  delay(1000);
  hand.detach();
}

void wifiError(WiFiManager *myWiFiManager) {
  setHand(INIT_HAND_POSITION);
}
