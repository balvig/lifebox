#ifndef Lifeboxes_raindrop_h
#define Lifeboxes_raindrop_h

#include "Arduino.h"
#include "RBD_Timer.h"

namespace Lifeboxes  {
  class Raindrop {
    public:
      Raindrop(int initX);
      int x;
      int y = 0;
      int active = 0;
      void update();
      void setFallingSpeed(int multiplier);
    private:
      RBD::Timer _updateTimer;
      void _updateY();
  };
}
#endif
