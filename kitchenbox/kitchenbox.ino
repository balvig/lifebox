// E-paper display
#include <GxEPD.h>
#include <GxGDE0213B1/GxGDE0213B1.cpp>      // 2.13" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// Libs
#include "Api.h"
#include "Net.h"
#include "Paper.h"

// Constants
const size_t JSON_BUFFER = JSON_OBJECT_SIZE(1) + 310; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://0ad33d4c.ngrok.io/kitchen";

// Variables
GxIO_Class io(SPI, SS, 0, 2);
GxEPD_Class display(io);
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Paper paper(&display);

void setup() {
  Serial.begin(115200);
  paper.renderText("Loading...", 1, false);
  net.connect();
  loadRecipe();
  net.disconnect();
}

void loop() {
}

void loadRecipe() {
  JsonObject& root = api.fetchJson();
  const String recipe = root["recipe"];
  paper.renderText(recipe);
}
