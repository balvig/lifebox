// Libs
#include "Api.h"
#include "Battery.h"
#include "ConfigurableNet.h"
#include "Sleep.h"
#include "Servo.h"

// Configuration
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 20; // http://arduinojson.org/assistant/
const String API_HOST = "http://lifeboxes.herokuapp.com";
const String API_ENDPOINT = API_HOST + "/recycle";
const String LOG_API_ENDPOINT = "http://api.thingspeak.com/update";
const String LOG_API_KEY = "92YAC5EVJF5ES69W";
const int INIT_HAND_POSITION = 180;
const int HAND_PIN = 4;
const int SLEEP_CYCLES = 6;
const int LOW_BATTERY_LEVEL = 800;

// Variables
Lifeboxes::ConfigurableNet net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Api logApi(LOG_API_ENDPOINT);
Lifeboxes::Sleep sleep(SLEEP_CYCLES);
Lifeboxes::Battery battery(LOW_BATTERY_LEVEL);
Servo hand;


// Main
void setup() {
  Serial.begin(115200);

  if(sleep.isTimeToWakeUp()) {
    run();
  }

  goToSleep();
}

void loop() {
}

void run() {
  connectToWifi();
  logBattery();
  //syncWithApi();
}

void connectToWifi() {
  net.setErrorCallback(wifiError);
  net.connect();
}

void syncWithApi() {
  JsonObject& root = api.fetchJson();
  int degrees = root["degrees"] | INIT_HAND_POSITION;
  setHand(degrees);
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

void logBattery() {
  const String body = "field1="+ String(battery.currentLevel()) + "&api_key=" + LOG_API_KEY;
  logApi.post(body);
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
