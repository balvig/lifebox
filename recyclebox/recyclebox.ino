// Libs
#include "Net.h"
#include "Api.h"
#include "Servo.h"

// Configuration
//const int SLEEPING_INTERVAL = 3600000; // 60 minutes
const int SLEEPING_INTERVAL = 10000; // 10 sec
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 20; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://b3ea1612.ngrok.io/recycle";

// Variables
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Servo hand;

// Main
void setup() {
  Serial.begin(115200);
  hand.attach(0);
  net.connect();
}

void loop() {
  updateState();
  delay(SLEEPING_INTERVAL);
}

// Private
void updateState() {
  Serial.println("Updating....");
 // if (net.connect()) {
    syncWithApi();
    //net.disconnect();
  //}
}

void syncWithApi() {
  JsonObject& root = api.fetchJson();
  int degrees = root["degrees"];
  Serial.print("Moving to: ");
  Serial.println(degrees);
  hand.write(degrees);
}
