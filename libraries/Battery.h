#ifndef Lifeboxes_battery_h
#define Lifeboxes_battery_h

#include "Arduino.h"

namespace Lifeboxes  {
  class Battery {
    public:
      Battery();
      uint32_t currentLevel();
  };
}
#endif
