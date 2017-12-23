// Libs
#include "Led.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Variables
RBD::Timer timer;
Lifebox::Led leds[] = { 12, 14, 27, 26, 25, 33, 32 };
Lifebox::Led debugLed(2); // Built-in LED

// Constants
#define REFRESH_INTERVAL 5000
#define LED_COUNT sizeof(leds) / sizeof(Lifebox::Led)

#define API_ENDPOINT "http://1b524aa3.ngrok.io/lifebox"

// Main
void setup() {
  delay(10);
  Serial.begin(115200);
  timer.setTimeout(REFRESH_INTERVAL);
  timer.restart();
  connectToWifi();
  updateState();
}

void loop() {
  if (timer.onRestart()) {
    updateState();
  }
  updateLights();
}

// Private
void updateLights() {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i].update();
  }
  debugLed.update();
}

void updateState() {
  if (WiFi.status() != WL_CONNECTED) {
    debugLed.blink(300, 300);
  }
  else {
    String results = fetchResults();
    char charBuf[50];
    results.toCharArray(charBuf, 50);
    char *token = strtok(charBuf, ",");
    int i = 0;
    
    while (token) {
      String tokenString = token;
      int state = tokenString.toInt();
      Serial.print(i);
      Serial.print(": ");
      Serial.print(state);
      Serial.println("");

      leds[i].setState(state);
      
      token = strtok(NULL, ",");
      i++;
    }

    debugLed.on();
  }
}

void connectToWifi() {
  WiFi.begin(WIFI_NAME, WIFI_PASS);
}

String fetchResults() {
  HTTPClient http;
  http.begin(API_ENDPOINT);

  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      http.end();
      Serial.println(payload);
      return payload;
    }
  }
  else {
    http.end();
    return "";
    //USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
}
