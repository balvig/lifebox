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
      int x;
      void setState(int state);
      Raindrop raindrops[2];
  };
}
#endif
