// E-paper display
#include <GxEPD.h>
//#include <GxGDEH029A1/GxGDEH029A1.cpp>      // 2.9" b/w
#include <GxGDEW029Z10/GxGDEW029Z10.cpp>    // 2.9" b/w/r
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// Libs
#include "Api.h"
#include "Net.h"
#include "Sleep.h"

// API config
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(2) + 310; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/inspire";

// Variables
GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io); // default selection of D4(=2), D2(=4)
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Sleep sleep(12);

// Fonts
#include "Font.h"
const GFXfont* LARGE_FONT = &Bookerly_Regular8pt7b;

void setup() {
  if(sleep.isTimeToWakeUp()) {
    initDisplay();
    
    if(net.connect()) {
      loadScreen();
      net.disconnect();
    }
    else {
      showStatus("Wifi Error");
    }
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

void showStatus(const String message) {
  display.fillScreen(GxEPD_WHITE);
  display.setCursor(0, 0);
  display.setFont();
  display.println(message);
  display.update();
}

void loadScreen() {
  JsonObject& root = api.fetchJson();
  const String text = root["text"];
  const String date = root["date"];

  // Clear screen
  display.fillScreen(GxEPD_WHITE);

  // Main text
  display.setCursor(0, 10);
  display.setFont(LARGE_FONT);
  display.println(text);

  // Date
  display.setFont();
  display.setCursor(232, 117);
  display.println(date);

  // Refresh
  display.update();
}
