// Libs
#include "Api.h"
#include "Battery.h"
#include "ConfigurableNet.h"
#include "Sleep.h"
#include "Servo.h"

// Configuration
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(2) + 30; // http://arduinojson.org/assistant/
const String API_HOST = "http://lifeboxes.herokuapp.com";
const String API_ENDPOINT = API_HOST + "/recycle";
const String LOG_API_KEY = "1I5V5U4297RGUKDQ";
const int INIT_HAND_POSITION = 180;
const int HAND_PIN = 13;
const int SERVO_POWER_PIN = 14;
const int DEFAULT_SLEEP_CYCLES = 1;
const int LOW_BATTERY_LEVEL = 800;

// Dev configuration
const uint64_t SECONDS = 10 * micros();
const uint64_t HOURS = 60 * 60 * SECONDS;
const uint64_t SLEEPING_INTERVAL = 1 * HOURS;
const uint64_t DEBUG_SLEEPING_INTERVAL = 5 * SECONDS;

// Debug display
#include "Adafruit_SSD1306.h"
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variables
Lifeboxes::ConfigurableNet net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Sleep sleep(DEFAULT_SLEEP_CYCLES, SLEEPING_INTERVAL);
//Lifeboxes::Sleep sleep(DEFAULT_SLEEP_CYCLES, DEBUG_SLEEPING_INTERVAL);
Lifeboxes::Battery battery;
Servo hand;

// Main
void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  clearScreen();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  log((String)ESP.getResetReason().c_str() + " (" + ESP.getResetInfoPtr()->reason + ")");
  
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
  connectToWifi();
  syncWithApi();
}

void syncWithApi() {
  log("Syncing with API");
  const String logValue = String(battery.currentLevel());
  JsonObject& root = api.fetchJson("?log_value=" + logValue + "&log_key=" + LOG_API_KEY);
  const int degrees = root["degrees"] | INIT_HAND_POSITION;
  setHand(degrees);
  const int cycles = root["cycles"] | DEFAULT_SLEEP_CYCLES;
  log((String)"New cycles: " + cycles);
  sleep.resetCyclesRemaining(cycles);
}

void connectToWifi() {
  log("Connecting to wifi");
  net.setErrorCallback(wifiError);
  net.connect();
}

void setHand(int degrees) {
  setServoPower(true);
  log((String)"Moving hand to: " + degrees);
  hand.attach(HAND_PIN);
  hand.write(degrees);
  log("Waiting..."); // To avoid interrupting hand
  delay(1000);
  setServoPower(false);
  log("Detaching");
  hand.detach();
}

void setServoPower(boolean powerOn) {
  if(powerOn) {
    log("Servo on");
    digitalWrite(SERVO_POWER_PIN, HIGH);
  }
  else {
    log("Servo off");
    digitalWrite(SERVO_POWER_PIN, LOW);
  }
}

void wifiError(WiFiManager *myWiFiManager) {
  log("Wifi error");
  log((String)"Access \"" + myWiFiManager->getConfigPortalSSID() + "\" wifi hotspot");
  log((String)"Browse to http://192.168.4.1");
  setHand(INIT_HAND_POSITION);
}

void goToSleep() {
  log((String)"Sleeping. Remain:" + sleep.cyclesRemaining);
  turnOffDisplay();
  sleep.goToSleep();
}

void log(String message) {
  if(display.getCursorY() > SCREEN_HEIGHT) {
    clearScreen();
  }

  Serial.println(message);
  display.println(message);
  display.display();
}

void clearScreen() {
  display.setCursor(0, 0);
  display.clearDisplay();
}

void turnOffDisplay() {
  delay(5000);
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}
