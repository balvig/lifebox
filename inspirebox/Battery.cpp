#include "Battery.h"

namespace Lifeboxes {
  Battery::Battery(uint32_t lowBatteryLimit) {
    _lowBatteryLimit = lowBatteryLimit;
  }

  bool Battery::low() {
    return _currentLevel() <= _lowBatteryLimit;
  }

  // Private
  uint32_t Battery::_currentLevel() {
    return analogRead(A0);
  }
}
