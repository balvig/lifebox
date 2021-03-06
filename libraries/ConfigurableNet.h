#ifndef Lifeboxes_configurable_net_h
#define Lifeboxes_configurable_net_h

#include "Arduino.h"
#include "ESP8266WiFi.h"          // ESP8266 Core WiFi Library
#include "DNSServer.h"            // Local DNS Server used for redirecting all requests to the configuration portal
#include "ESP8266WebServer.h"     // Local WebServer used to serve the configuration portal
#include "WiFiManager.h"          // https://github.com/tzapu/WiFiManager

namespace Lifeboxes  {
  class ConfigurableNet {
    public:
      ConfigurableNet();
      void connect();
      void setErrorCallback(void (*func)(WiFiManager*));
      void disconnect();
    private:
      WiFiManager _wifiManager;
  };
}
#endif
