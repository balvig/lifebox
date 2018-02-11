#include "Sky.h"

namespace Lifeboxes {
  Sky::Sky(int x) 
  : raindrops{x, x+1, x, x+1} {
  }

  void Sky::setState(int state) {
    switch (state) {
      case 1:
        _setLightRain();
        break;
      case 2:
        _setHeavyRain();
        break;
      default:
        _stopRain();
        break;
    }
  }

  // private
  void Sky::_stopRain() {
    for (auto &raindrop : raindrops) {
      raindrop.active = false;
    }
  }

  void Sky::_setLightRain() {
    for (auto &raindrop : raindrops) {
      raindrop.intensity = Raindrop::SLOW;
    }

    raindrops[0].active = true;
    raindrops[1].active = true;
    raindrops[2].active = false;
    raindrops[3].active = false;
  }

  void Sky::_setHeavyRain() {
    for (auto &raindrop : raindrops) {
      raindrop.intensity = Raindrop::FAST;
    }

    raindrops[0].active = true;
    raindrops[1].active = true;
    raindrops[2].active = true;
    raindrops[3].active = true;
  }
}
