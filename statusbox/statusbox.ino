// Libs
#include "RBD_Timer.h"
#include "RBD_Button.h"
#include "Adafruit_SSD1306.h"
#include "Net.h"
#include "Api.h"

// Configuration
const int REFRESH_INTERVAL = 60000; // 60 seconds
const int INACTIVITY_INTERVAL = 20000; // 20 seconds
const size_t JSON_BUFFER = JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + 130; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/status";
//const String API_ENDPOINT = "http://8afe6a43.ngrok.io/status";
const int NUM_OF_SCREENS = 3;

// Variables
RBD::Timer updateTimer;
RBD::Timer autoSleepTimer;
RBD::Button leftButton(0);
RBD::Button rightButton(2);
Adafruit_SSD1306 lcd;
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
    updateTimer.setTimeout(REFRESH_INTERVAL);
    updateTimer.restart();
  }
  else {
    updateTimer.stop();
    showError();
  }
  autoSleepTimer.setTimeout(INACTIVITY_INTERVAL);
  autoSleepTimer.restart();
}

void loop() {
  if (leftButton.onPressed()) {
    previousScreen();
    autoSleepTimer.restart();
  }
  if (rightButton.onPressed()) {
    nextScreen();
    autoSleepTimer.restart();
  }

  if (autoSleepTimer.onExpired()) {
    goToSleep();
  }
  
  if (updateTimer.onRestart()) {
    updateState();
  }
}

// Private
void updateState() {
  syncWithApi();
}

void syncWithApi() {
  JsonObject& json = api.fetchJson();
  JsonArray&data = json["screens"];
  int dataSize = data.size();

  for (int i = 0; i < dataSize; i++) {
    String screen = data[i];
    screens[i] = screen;
  }

  if (dataSize > 0) {
    showCurrentScreen();
  }
  else {
    showError();
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

  lcdMessage(currentScreen);
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
  lcdMessage("Error\nWifi: " + String(net.wifiStatus));
}

void goToSleep() {
  lcd.ssd1306_command(SSD1306_DISPLAYOFF);
  ESP.deepSleep(0);
}
