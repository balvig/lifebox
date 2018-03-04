// Libs
#include "Net.h"
#include "Api.h"
#include "Servo.h"

// Configuration
const int SLEEPING_INTERVAL = 60 * 60 * 24; // 60 minutes * 24
//const int SLEEPING_INTERVAL = 5; // 5 sec
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 20; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/recycle";
//const String API_ENDPOINT = "http://a0e839bc.ngrok.io/recycle";
const int INIT_HAND_POSITION = 180;
const int HAND_PIN = 2;

// Variables
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Servo hand;


// Main
void setup() {
  Serial.begin(115200);
  syncWithApi();
  goToSleep(); // D0-RST on Wemos, remove when uploading...
}

void loop() {
}

void syncWithApi() {
  net.connect(); // For some reason Wemos D1 won't reconnect after disconnect so keeping connected
  JsonObject& root = api.fetchJson();
  int degrees = root["degrees"] | INIT_HAND_POSITION;
  Serial.println(degrees);
  hand.attach(HAND_PIN);
  hand.write(degrees);
  delay(1000);
  hand.detach();
}

void goToSleep() {
  ESP.deepSleep(SLEEPING_INTERVAL * 1000000);
}
