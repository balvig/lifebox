#include "ConfigurableNet.h"

namespace Lifeboxes {
  
  ConfigurableNet::ConfigurableNet() {}

  void ConfigurableNet::connect() {
    _wifiManager.autoConnect("Lifebox");    
  }

  void ConfigurableNet::setErrorCallback(void (*callback)(WiFiManager *myWiFiManager)) {
    _wifiManager.setAPCallback(callback);
  }

  void ConfigurableNet::disconnect() {
    WiFi.mode(WIFI_OFF);
  }
}
