#ifndef Lifeboxes_net_h
#define Lifeboxes_net_h

#include "Arduino.h"
#include "Config.h"

#ifdef ESP_PLATFORM
  #include "WiFi.h"
#else
  #include "ESP8266WiFi.h"
#endif  

namespace Lifeboxes  {
  class Net {
    public:
      Net();
      int wifiStatus;
      bool connect(int tries = 10);
      void disconnect();
  };
}
#endif
