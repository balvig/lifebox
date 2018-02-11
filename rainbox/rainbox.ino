// Libs
#include "RBD_Timer.h"
#include "WEMOS_Matrix_GFX.h"
#include "Net.h"
#include "Api.h"
#include "Sky.h"

// Configuration
const int UPDATE_INTERVAL = 1800000; // 30 minutes
const size_t JSON_BUFFER = JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(1) + 20; // http://arduinojson.org/assistant/
const String API_ENDPOINT = "http://lifeboxes.herokuapp.com/rain";
//const String API_ENDPOINT = "http://11d61090.ngrok.io/rain";

// Images
static const uint8_t PROGMEM
  checkBmp[] = { 
    B00000000,
    B00000001,
    B00000010,
    B00000100,
    B10001000,
    B01010000,
    B00100000,
    B00000000
  },
  downBmp[] = { 
    B00000000,
    B00011000,
    B00011000,
    B00011000,
    B01111110,
    B00111100,
    B00011000,
    B00000000
  };

// Variables    
MLED matrix(5);
RBD::Timer updateTimer;
Lifeboxes::Net net;
Lifeboxes::Api api(API_ENDPOINT, JSON_BUFFER);
Lifeboxes::Sky skies[] = { 0, 2, 4, 6 };

// Main
void setup() {
  Serial.begin(115200);

  showUpdating();
  
  if (net.connect()) { // Can't get wemos to reconnect after disconnect so leaving on fow now
    updateState();
    showSuccess();
    delay(2000);
    updateTimer.setTimeout(UPDATE_INTERVAL);
    updateTimer.restart();
  }
  else {
    showWifiState();
  }
}

void loop() {
  if (updateTimer.onRestart()) {
    updateState();
    // conserveBattery();
  }

  animate();
}

// Private
void updateState() {
  syncWithApi();
}

void syncWithApi() {
  JsonObject& json = api.fetchJson();

  syncSkies(json["states"]);
}

void syncSkies(JsonArray& data) {
  int dataSize = data.size();
  
  for (int i = 0; i < dataSize; i++) {
    int state = data[i];
    skies[i].setState(state);
  }
}

void animate() {
  matrix.clear();

  for (auto &sky : skies) {
    for (auto &raindrop : sky.raindrops) {
      if (raindrop.active) {
        matrix.drawPixel(raindrop.x, raindrop.y, 1);
        raindrop.update();
      }
    }
  }

  matrix.writeDisplay();
}

void showUpdating() {
  matrix.clear();
  matrix.drawBitmap(0, 0, downBmp, 8, 8, LED_ON);
  matrix.writeDisplay();
}

void showSuccess() {
  matrix.clear();
  matrix.drawBitmap(0, 0, checkBmp, 8, 8, LED_ON);
  matrix.writeDisplay();
}

void showWifiState() {
  matrix.clear();
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setCursor(1,1);
  matrix.print(net.wifiStatus);
  matrix.writeDisplay();
}

void conserveBattery() {
  if (needRealtimeUpdates()) {
    return;
  }
  else {
    //deepsleep
    //delay(REALTIME_LOOP_SLEEP);
  }
}
    
boolean needRealtimeUpdates() {
  return true;
}
