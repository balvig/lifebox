#ifndef Lifeboxes_sleep_h
#define Lifeboxes_sleep_h

extern "C" {
#include "user_interface.h"
}
#include "Arduino.h"

namespace Lifeboxes  {
  const uint64_t MAX_SLEEPING_INTERVAL = ESP.deepSleepMax();

  class Sleep {
    public:
      Sleep(uint32_t cyclesToSleep, uint64_t sleepingInterval = MAX_SLEEPING_INTERVAL);
      bool isTimeToWakeUp();
      void resetCyclesRemaining(uint32_t newValue);
      void goToSleep();
    private:
      uint32_t _cyclesRemaining;
      uint32_t _defaultCyclesToSleep;
      uint64_t _sleepingInterval;
      bool _wasResetFromSleepUp();
      void _updateCyclesRemaining();
      void _readCyclesRemaining();
      void _writeCyclesRemaining();
  };
}
#endif
