// E-paper display
#include <GxEPD.h>
//#include <GxGDEH029A1/GxGDEH029A1.cpp>      // 2.9" b/w
#include <GxGDEW029Z10/GxGDEW029Z10.cpp>    // 2.9" b/w/r
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// Libs
#include "Api.h"
#include "Battery.h"
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
Lifeboxes::Sleep sleep(1);
Lifeboxes::Battery battery(700);

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

  const uint8_t margin = 10;
  const uint8_t batteryX = 0 + margin / 4;
  const uint8_t batteryY = bottom - margin;
  const uint8_t batteryWidth = 25;
  const uint8_t batteryHeight = batteryWidth / 3;
  const uint8_t tipHeight = batteryHeight / 2;
  const uint8_t tipWidth = 2;
  const uint8_t tipLeadUp = (batteryHeight - tipHeight) / 2;
  const uint8_t padding = 2;
  const uint8_t markerWidth = batteryWidth / 3 - padding;
  
  display.drawLine(batteryX, batteryY, batteryX + batteryWidth, batteryY, GxEPD_BLACK);
  display.drawLine(batteryX, batteryY + batteryHeight, batteryX + batteryWidth, batteryY + batteryHeight, GxEPD_BLACK);
  display.drawLine(batteryX, batteryY, batteryX, batteryY + batteryHeight, GxEPD_BLACK);

  
  display.drawLine(batteryX + batteryWidth, batteryY, batteryX + batteryWidth, batteryY + tipLeadUp, GxEPD_BLACK);
  display.drawLine(batteryX + batteryWidth, batteryY + batteryHeight, batteryX + batteryWidth, batteryY + batteryHeight - tipLeadUp, GxEPD_BLACK);

  display.drawLine(batteryX + batteryWidth, batteryY + tipLeadUp, batteryX + batteryWidth + tipWidth, batteryY + tipLeadUp, GxEPD_BLACK);
  display.drawLine(batteryX + batteryWidth, batteryY + batteryHeight - tipLeadUp, batteryX + batteryWidth + tipWidth, batteryY + batteryHeight - tipLeadUp, GxEPD_BLACK);

  display.drawLine(batteryX + batteryWidth + tipWidth, batteryY + tipLeadUp, batteryX + batteryWidth + tipWidth, batteryY + tipLeadUp + tipHeight, GxEPD_BLACK);

  display.fillRect(batteryX + padding, batteryY + padding, markerWidth, batteryHeight - (padding * 2), GxEPD_BLACK);
  
  
  //display.drawLine(batteryX + batteryWidth, batteryY, batteryX + batteryWidth, batteryY + batteryHeight, GxEPD_BLACK);
  
  //display.drawRect(0, bottom, 100, 10, GxEPD_BLACK);
  

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
