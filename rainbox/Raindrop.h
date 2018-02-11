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
      uint8_t intensity;
      boolean active = false;
      void update();
      void restart();
    private:
      RBD::Timer _updateTimer;
      const uint8_t MATRIX_ROWS = 8;
      const uint8_t WAITING_ROWS_MAX = 5;
      int FALLING_INTERVAL_MIN = 100;
      int FALLING_INTERVAL_MAX = 300;
      uint8_t rows = MATRIX_ROWS;
      void _updatePosition();
  };
}
#endif
