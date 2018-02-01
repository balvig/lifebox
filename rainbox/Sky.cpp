#include "Sky.h"

namespace Lifeboxes {
  Sky::Sky(int x) 
  : raindrops{x, x+1, x, x+1} {
  }

  void Sky::setState(int state) {
    switch (state) {
      case 1:
        startHeavyRain();
        break;
      case 2:
        startLightRain();
        break;
      default:
        stopRain();
        break;
    }
  }

  // private

  void Sky::stopRain() {
    for (auto &raindrop : raindrops) {
      raindrop.active = 0;
    }
  }

  void Sky::startLightRain() {
    for (auto &raindrop : raindrops) {
      raindrop.setFallingSpeed(1);
    }

    raindrops[0].active = 1;
    raindrops[1].active = 1;
    raindrops[2].active = 0;
    raindrops[3].active = 0;
  }

  void Sky::startHeavyRain() {
    for (auto &raindrop : raindrops) {
      raindrop.setFallingSpeed(2);
    }

    raindrops[0].active = 1;
    raindrops[1].active = 1;
    raindrops[2].active = 1;
    raindrops[3].active = 1;
  }
}
