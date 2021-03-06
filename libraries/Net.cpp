#include "Net.h"

namespace Lifeboxes {
  const int WIFI_POLLING_RATE = 1200;
  
  Net::Net() {}

  bool Net::connect(int tries) {
    WiFi.begin(LIFEBOXES_WIFI_NAME, LIFEBOXES_WIFI_PASS);
  
    for(int i = 0; i < tries; i++) {
      wifiStatus = WiFi.status();

      delay(WIFI_POLLING_RATE); // blocking all the things

      if (wifiStatus == WL_CONNECTED) {
        return true;
      }
    }

    return false;
  }

  void Net::disconnect() {
    WiFi.disconnect();
  }
}
