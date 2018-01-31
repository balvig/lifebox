// Libs
#include "RBD_Timer.h"
#include "Adafruit_SSD1306.h"
#include "Net.h"
#include "Api.h"

// Configuration
//const int SLEEPING_INTERVAL = 1800000; // 30 minutes
const int SLEEPING_INTERVAL = 60000; // 60 seconds
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 50; // http://arduinojson.org/assistant/
//const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/weather";
const String API_ENDPOINT = "http://6971e5fa.ngrok.io/status";


// Variables
RBD::Timer updateTimer;
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 lcd(OLED_RESET);
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);


// Main
void setup() {
  Serial.begin(115200);
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  lcdMessage("Connecting");
  updateTimer.setTimeout(SLEEPING_INTERVAL);
  updateTimer.restart();
  updateState();
}

void loop() {
  if (updateTimer.onRestart()) {
    updateState();
  }
}

// Private
void updateState() {
  if (net.connect()) {
    syncWithApi();
    net.disconnect();
  }
  else {
    lcdMessage("Error: " + net.wifiStatus);
  }
}

void syncWithApi() {
  JsonObject& json = api.fetchJson();

  lcdMessage(json["message"]);
}

void lcdMessage(String message) {
  lcd.setCursor(0, 0);
  lcd.clearDisplay();
  lcd.setTextSize(1);
  lcd.setTextColor(WHITE);
  lcd.println(message);
  lcd.display();
}
