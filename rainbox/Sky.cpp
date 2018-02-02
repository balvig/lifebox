#include "Sky.h"

namespace Lifeboxes {
  Sky::Sky(int x) 
  : raindrops{x, x+1, x, x+1} {
  }

  void Sky::setState(int state) {
    switch (state) {
      case 1:
        startLightRain();
        break;
      case 2:
        startHeavyRain();
        break;
      default:
        stopRain();
        break;
    }
  }

  // private

  void Sky::stopRain() {
    for (auto &raindrop : raindrops) {
      raindrop.active = false;
    }
  }

  void Sky::startLightRain() {
    for (auto &raindrop : raindrops) {
      raindrop.setFallingSpeed(Raindrop::SLOW);
    }

    raindrops[0].active = true;
    raindrops[1].active = true;
    raindrops[2].active = false;
    raindrops[3].active = false;
  }

  void Sky::startHeavyRain() {
    for (auto &raindrop : raindrops) {
      raindrop.setFallingSpeed(Raindrop::FAST);
    }

    raindrops[0].active = true;
    raindrops[1].active = true;
    raindrops[2].active = true;
    raindrops[3].active = true;
  }
}
