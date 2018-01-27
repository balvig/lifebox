#include "Sky.h"

namespace Lifeboxes {
  Sky::Sky(int x) 
  : raindrops{x, x+1} {
  }

  void Sky::setState(int state) {
    for (auto &raindrop : raindrops) {
      if (state == 0) {
        raindrop.active = 0;
      }
      else if (state == 1) {
        raindrop.setFallingSpeed(2);
        raindrop.active = 1;
      }
      else if (state == 2) {
        raindrop.setFallingSpeed(1);
        raindrop.active = 1;
      }
    }
  }
}
