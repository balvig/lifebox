#include "Battery.h"

namespace Lifeboxes {
  Battery::Battery(uint32_t lowBatteryLimit) {
    _lowBatteryLimit = lowBatteryLimit;
  }

  bool Battery::low() {
    return currentLevel() <= _lowBatteryLimit;
  }

  uint32_t Battery::currentLevel() {
    return analogRead(A0);
  }
}
