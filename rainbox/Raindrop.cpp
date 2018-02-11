#include "Raindrop.h"

namespace Lifeboxes {
  Raindrop::Raindrop(uint8_t initX)
  : _updateTimer() {
    x = initX;
    restart();
  }
  
  void Raindrop::update() {
    if (_updateTimer.onRestart()) {
      _updatePosition();
    }
  }

  void Raindrop::restart() {
    rows = MATRIX_ROWS + random(0, WAITING_ROWS_MAX);
    int fallingInterval = random(FALLING_INTERVAL_MIN, FALLING_INTERVAL_MAX) * intensity;
    _updateTimer.setTimeout(fallingInterval);
  }

  // Private
  void Raindrop::_updatePosition() {
    y++;
    if (y >= rows) {
      y = 0;
      restart();
    }
  }
}
