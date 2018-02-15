// E-paper display
#include <GxEPD.h>
//#include <GxGDE0213B1/GxGDE0213B1.cpp>      // 2.13" b/w
#include <GxGDEW029Z10/GxGDEW029Z10.cpp>    // 2.9" b/w/r
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// Libs
#include "Api.h"
#include "Net.h"

// Constants
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 310; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/kitchen";

// Variables

//GxIO_Class io(SPI, SS, 4, 2); 
//GxEPD_Class display(io, 2, 5);

GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io); // default selection of D4(=2), D2(=4)


//#include <Fonts/FreeSerif9pt7b.h>
//const GFXfont* FONT = &FreeSerif9pt7b;


Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);

void setup() {
  Serial.begin(115200);

  display.init();
  display.setRotation(1);
    //_display->setFont(FONT);
  display.setTextColor(GxEPD_BLACK);
  delay(1000);
  
  renderText("\n\n\n\n\n\n\n  Loading recipe...", false);
  if(net.connect()) {
    loadRecipe();
    net.disconnect();
  }
  else {
    renderText("\n\n\n\n\n\n\n  Wifi Error", false);
  }

  ESP.deepSleep(0); // sleep until reset
}

void loop() {
}

void loadRecipe() {
  JsonObject& root = api.fetchJson();
  const String recipe = root["recipe"];
  renderText(recipe, true);
}

void renderText(const String text, const boolean fullRefresh) {
  int page = 1;
  int offset = (page - 1) * GxEPD_HEIGHT;
  
  display.setCursor(0, -offset);
  display.fillScreen(GxEPD_WHITE);
  display.println(text);

  if (fullRefresh) {
    display.update();
  }
  else {
    display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
  }   
}
