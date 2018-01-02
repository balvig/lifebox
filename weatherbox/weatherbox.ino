// Libs
#include "Adafruit_SSD1306.h"
#include "Led.h"
#include "Net.h"
#include "Api.h"

// Configuration
const int SLEEPING_INTERVAL = 1800000; // 30 minutes
const int LOOP_SLEEP = 25;
const size_t JSON_BUFFER = JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(3) + 80; // http://arduinojson.org/assistant/
//const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/weather";
const String API_ENDPOINT = "http://a7df2682.ngrok.io/weather";

// Variables
RBD::Timer updateTimer;
Adafruit_SSD1306 lcd;
Lifeboxes::Led debugLed(2); // Built-in LED
Lifeboxes::Led leds[] = { 12, 14, 27 };
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
const int LED_COUNT = sizeof(leds) / sizeof(Lifeboxes::Led);


// Main
void setup() {
  Serial.begin(115200);
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // 128x32
  lcdMessage("Initializing...");
  testLights();
  updateTimer.setTimeout(SLEEPING_INTERVAL);
  updateTimer.restart();
  lcdMessage("Connecting...");
  updateState();
}

void loop() {
  if (updateTimer.onRestart()) {
    updateState();
  }
  updateLights();
  delay(LOOP_SLEEP);
}

// Private
void updateState() {
  debugLed.on();

  if (net.connect()) {
    syncWithApi();
    net.disconnect();
  }
  else {
    lcdMessage("Wifi connection error");
  }
  
  debugLed.off();
}

void syncWithApi() {
  JsonObject& json = api.fetchJson();

  syncLeds(json["led_values"]);
  syncLcd(json["temperatures"]);
}

void syncLeds(JsonArray& data) {
  int dataSize = data.size(); 
  for (int i = 0; i < dataSize; i++) {
    int state = data[i];
    leds[i].setState(state);
  }
}

void syncLcd(JsonArray& data) {
  lcd.setCursor(0, 0);
  lcd.clearDisplay();

  for (auto &temperature : data) {
    printTemperature(temperature);
  } 
  
  lcd.display();
}

void printTemperature(String degrees) {
  lcd.setTextSize(2);
  lcd.setTextColor(WHITE);
  lcd.print(degrees);
  
  lcd.setTextSize(1);
  lcd.print((char)247);
  
  lcd.setTextSize(2);
  lcd.println("C");
}

void lcdMessage(String message) {
  lcd.setCursor(0, 0);
  lcd.clearDisplay();
  lcd.setTextSize(1);
  lcd.setTextColor(WHITE);
  lcd.println(message);
  lcd.display();
}

void testLights() {
  for (auto &led : leds) {
    led.on();
  }

  delay(1000);
  
  for (auto &led : leds) {
    led.off();
  }
}

void updateLights() {
  for (auto &led : leds) {
    led.update();
  }
  debugLed.update();
}
