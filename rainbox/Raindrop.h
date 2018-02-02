#ifndef Lifeboxes_raindrop_h
#define Lifeboxes_raindrop_h

#include "Arduino.h"
#include "RBD_Timer.h"

namespace Lifeboxes  {
  class Raindrop {
    public:
      Raindrop(uint8_t initX);
      static const uint8_t FAST = 1;
      static const uint8_t SLOW = 2;
      uint8_t x;
      uint8_t y = 0;
      boolean active = false;
      void update();
      void setFallingSpeed(int multiplier);
    private:
      RBD::Timer _updateTimer;
      const uint8_t ROWS = 8;
      void _updateY();
  };
}
#endif
