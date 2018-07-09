#ifndef Lifeboxes_battery_h
#define Lifeboxes_battery_h

#include "Arduino.h"

namespace Lifeboxes  {
  class Battery {
    public:
      Battery(uint32_t lowBatteryLimit);
      bool low();
    private:
      uint32_t _lowBatteryLimit;
      uint32_t _currentLevel();
  };
}
#endif
