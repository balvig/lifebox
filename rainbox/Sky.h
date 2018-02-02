#ifndef Lifeboxes_sky_h
#define Lifeboxes_sky_h

#include "Arduino.h"
#include "ArduinoJson.h"
#include "RBD_Timer.h"
#include "Raindrop.h"

namespace Lifeboxes  {
  class Sky {
    public:
      Sky(int initX);
      void setState(int state);
      Raindrop raindrops[4];
    private:
      void stopRain();
      void startLightRain();
      void startHeavyRain();
  };
}
#endif
