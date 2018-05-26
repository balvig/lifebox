// E-paper display
#include <GxEPD.h>
//#include <GxGDEH029A1/GxGDEH029A1.cpp>      // 2.9" b/w
#include <GxGDEW029Z10/GxGDEW029Z10.cpp>    // 2.9" b/w/r
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// Libs
#include "Api.h"
#include "ConfigurableNet.h"
#include "Sleep.h"

// API config
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(2) + 310; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/inspire";

// Variables
GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io); // default selection of D4(=2), D2(=4)
Lifeboxes::ConfigurableNet net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Sleep sleep(12);

// Fonts
#include "Font.h"
const GFXfont* LARGE_FONT = &Bookerly_Regular8pt7b;

void setup() {
  if(sleep.isTimeToWakeUp()) {
    initDisplay();
    net.setErrorCallback(wifiError);
    net.connect();
    loadScreen();
  }
  
  sleep.goToSleep();
}

void loop() {
}

void initDisplay() {
  display.init();
  display.setRotation(1);
  display.setTextColor(GxEPD_BLACK);
  delay(1000);
}

void loadScreen() {
  JsonObject& root = api.fetchJson();
  const String text = root["text"];
  const String date = root["date"];
  const uint8_t top = 10;
  const uint8_t bottom = 117;

  // Clear screen
  display.fillScreen(GxEPD_WHITE);

  // Main text
  display.setCursor(0, top);
  display.setFont(LARGE_FONT);
  display.println(text);

  // Date
  display.setFont();
  display.setCursor(232, bottom);
  display.println(date);

  // Battery
  display.setCursor(0, bottom);
  display.println("Battery: " + analogRead(A0));

  // Refresh
  display.update();

  // Give slow displays a chance to finish
  delay(3000);
}

void wifiError(WiFiManager *myWiFiManager) {
  const String message = "Wifi connection error. To configure: \n\n- Access \"" + myWiFiManager->getConfigPortalSSID() + "\" wifi hotspot. \n- Browse to 192.168.4.1";
  showStatus(message);
}

void showStatus(const String message) {
  display.fillScreen(GxEPD_WHITE);
  display.setCursor(0, 0);
  display.setFont();
  display.println(message);
  display.update();
}
