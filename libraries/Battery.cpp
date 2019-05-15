#include "Battery.h"

namespace Lifeboxes {
  Battery::Battery() {}

  uint32_t Battery::currentLevel() {
    return analogRead(A0);
  }
}
