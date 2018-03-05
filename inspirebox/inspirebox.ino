// E-paper display
#include <GxEPD.h>
#include <GxGDEW029Z10/GxGDEW029Z10.cpp>    // 2.9" b/w/r
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// Libs
#include "Api.h"
#include "Net.h"
#include "Sleep.h"

// API config
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 310; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/inspire";

// Variables
GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io); // default selection of D4(=2), D2(=4)
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Sleep sleep(4);

// Fonts
#include <Fonts/FreeSans9pt7b.h>
const GFXfont* FONT = &FreeSans9pt7b;

void setup() {
  if(sleep.isTimeToWakeUp()) {
    initDisplay();
    
    if(net.connect()) {
      loadScreen();
      net.disconnect();
    }
    else {
      renderText("Wifi Error");
    }
  }
  
  sleep.goToSleep();
}

void loop() {
}

void initDisplay() {
  display.init();
  display.setRotation(1);
  display.setFont(FONT);
  display.setTextColor(GxEPD_BLACK);
  delay(1000);
}

void loadScreen() {
  JsonObject& root = api.fetchJson();
  const String text = root["text"];
  renderText(text);
}

void renderText(const String text) {  
  display.setCursor(0, 20);
  display.fillScreen(GxEPD_WHITE);
  display.println(text);
  display.update();
}
