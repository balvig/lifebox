#include "Raindrop.h"

namespace Lifeboxes {

  const int ROWS = 8;
  
  Raindrop::Raindrop(int initX)
  : _updateTimer() {
    x = initX;
  }
  
  void Raindrop::update() {
    if (_updateTimer.onRestart()) {
      _updateY();
    }
  }

  void Raindrop::setFallingSpeed(int multiplier) {
    int fallingSpeed = random(100, 300) * multiplier;
    _updateTimer.setTimeout(fallingSpeed);
  }

  // Private

  void Raindrop::_updateY() {
    y++;
    if (y >= ROWS) {
      y = 0;
    }
  }
}

