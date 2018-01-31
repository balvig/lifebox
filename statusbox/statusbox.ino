// Libs
#include "RBD_Timer.h"
#include "RBD_Button.h"
#include "Adafruit_SSD1306.h"
#include "Net.h"
#include "Api.h"

// Configuration
//const int SLEEPING_INTERVAL = 1800000; // 30 minutes
const int SLEEPING_INTERVAL = 60000; // 60 seconds
const size_t JSON_BUFFER = JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(1) + 80; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/status";
//const String API_ENDPOINT = "http://8afe6a43.ngrok.io/status";
const int NUM_OF_SCREENS = 3;


// Variables
RBD::Timer updateTimer;
RBD::Button leftButton(0);
RBD::Button rightButton(2);
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 lcd(OLED_RESET);
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);

int currentScreenIndex = 0;
String screens[NUM_OF_SCREENS];


// Main
void setup() {
  Serial.begin(115200);
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  lcdMessage("Connecting");

  if (net.connect()) {
    updateState();
    updateTimer.setTimeout(SLEEPING_INTERVAL);
    updateTimer.restart();
  }
  else {
    updateTimer.stop();
    showError();
  }
}

void loop() {
  if (leftButton.onPressed()) {
    previousScreen();
  }
  if (rightButton.onPressed()) {
    nextScreen();
  }
  
  if (updateTimer.onRestart()) {
    updateState();
  }
}

// Private
void updateState() {
  syncWithApi();
  showCurrentScreen();
}

void syncWithApi() {
  JsonObject& json = api.fetchJson();
  JsonArray&data = json["screens"];
  int dataSize = data.size(); 
  
  for (int i = 0; i < dataSize; i++) {
    String screen = data[i];
    screens[i] = screen;
  }
}

void previousScreen() {
  currentScreenIndex--;

  if (currentScreenIndex < 0) {
    currentScreenIndex = NUM_OF_SCREENS -1;
  }

  showCurrentScreen();
}

void nextScreen() {
  currentScreenIndex++;

  if (currentScreenIndex >= NUM_OF_SCREENS) {
    currentScreenIndex = 0;
  }

  showCurrentScreen();
}

void showCurrentScreen() {
  String currentScreen = screens[currentScreenIndex];

  if (currentScreen) {
    lcdMessage(currentScreen);
  }
  else {
    showError();
  }
}

void lcdMessage(String message) {
  lcd.setCursor(0, 0);
  lcd.clearDisplay();
  lcd.setTextSize(1);
  lcd.setTextColor(WHITE);
  lcd.print(message);
  lcd.display();
}

void showError() {
  lcdMessage("Error: " + String(net.wifiStatus));
}
