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
const int INIT_HAND_POSITION = 180;
const int HAND_PIN = 13;
const int SERVO_POWER_PIN = 14;
const int SLEEP_CYCLES = 2;
const int LOW_BATTERY_LEVEL = 800;

// Dev configuration
const double SECONDS = 10 * micros();
const double DEBUG_SLEEPING_INTERVAL = 10 * SECONDS;

// Variables
Lifeboxes::ConfigurableNet net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Sleep sleep(SLEEP_CYCLES);
Lifeboxes::Battery battery(LOW_BATTERY_LEVEL);
Servo hand;


// Main
void setup() {
  Serial.begin(115200);
  pinMode(SERVO_POWER_PIN, OUTPUT);
  setServoPower(false);

  if(sleep.isTimeToWakeUp()) {
    run();
  }

  goToSleep();
}

void loop() {
}

void run() {
  syncWithApi();
}

void syncWithApi() {
  connectToWifi();
  const String logValue = String(battery.currentLevel());
  JsonObject& root = api.fetchJson("?log_value=" + logValue);
  int degrees = root["degrees"] | INIT_HAND_POSITION;
  setHand(degrees);
}

void connectToWifi() {
  net.setErrorCallback(wifiError);
  net.connect();
}

void setHand(int degrees) {
  setServoPower(true);
  Serial.print("Moving hand to: ");
  Serial.println(degrees);
  hand.attach(HAND_PIN);
  hand.write(degrees);
  Serial.println("Waiting to avoid interrupting hand");
  delay(1000);
  setServoPower(false);
  Serial.println("Detaching");
  hand.detach();
}

void setServoPower(boolean powerOn) {
  if(powerOn) {
    Serial.println("Powering up servo");
    digitalWrite(SERVO_POWER_PIN, HIGH);
  }
  else {
    Serial.println("Powering down servo");
    digitalWrite(SERVO_POWER_PIN, LOW);
  }
}

void wifiError(WiFiManager *myWiFiManager) {
  const String message = "Wifi connection error. To configure: \n\n- Access \"" + myWiFiManager->getConfigPortalSSID() + "\" wifi hotspot. \n- Browse to http://192.168.4.1";
  Serial.println(message);
  setHand(INIT_HAND_POSITION);
}

void goToSleep() {
  Serial.println("Sleeping.");
  sleep.goToSleep();
}
