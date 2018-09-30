// Libs
#include "Api.h"
#include "Battery.h"
#include "ConfigurableNet.h"
#include "Sleep.h"
#include "Servo.h"

// Configuration
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 20; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/recycle";
const int INIT_HAND_POSITION = 180;
const int HAND_PIN = 4;
const int SLEEP_CYCLES = 6;
const int LOW_BATTERY_LEVEL = 800;

// Variables
Lifeboxes::ConfigurableNet net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Sleep sleep(SLEEP_CYCLES);
Lifeboxes::Battery battery(LOW_BATTERY_LEVEL);
Servo hand;


// Main
void setup() {
  Serial.begin(115200);

  if(battery.low()) {
    batteryError();
  }
  else if(sleep.isTimeToWakeUp()) {
    run();
  }
}

void loop() {
}

void run() {
  syncWithApi();
}

void syncWithApi() {
  net.setErrorCallback(wifiError);
  net.connect();
  JsonObject& root = api.fetchJson();
  int degrees = root["degrees"] | INIT_HAND_POSITION;
  setHand(degrees);
  goToSleep();
}

void setHand(int degrees) {
  Serial.print("Moving hand to: ");
  Serial.println(degrees);
  hand.attach(HAND_PIN);
  hand.write(degrees);
  delay(2000);
  hand.detach();
}

void wifiError(WiFiManager *myWiFiManager) {
  const String message = "Wifi connection error. To configure: \n\n- Access \"" + myWiFiManager->getConfigPortalSSID() + "\" wifi hotspot. \n- Browse to 192.168.4.1";
  Serial.println(message);
  setHand(INIT_HAND_POSITION);
}

void batteryError() {
  Serial.print("Low battery: ");
  Serial.println(battery.currentLevel());
  setHand(INIT_HAND_POSITION);
  goToSleep();
}

void goToSleep() {
  sleep.goToSleep();
  Serial.println("Sleeping.");
}
