// Libs
#include "Net.h"
#include "Api.h"
#include "Servo.h"

// Configuration
const int SLEEPING_INTERVAL = 3600000; // 60 minutes
//const int SLEEPING_INTERVAL = 10000; // 10 sec
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 20; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/recycle";

// Variables
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Servo hand;

// Main
void setup() {
  Serial.begin(115200);
  hand.attach(16);
  net.connect(); // For some reason Wemos D1 won't reconnect after disconnect so keeping connected
}

void loop() {
  syncWithApi();
  delay(SLEEPING_INTERVAL);
}

void syncWithApi() {
  JsonObject& root = api.fetchJson();
  int degrees = root["degrees"];
  hand.write(degrees);
}
