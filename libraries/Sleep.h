#ifndef Lifeboxes_sleep_h
#define Lifeboxes_sleep_h

extern "C" {
#include "user_interface.h"
}
#include "Arduino.h"

namespace Lifeboxes  {
  const uint64_t DEFAULT_SLEEPING_INTERVAL = ESP.deepSleepMax();

  class Sleep {
    public:
      Sleep(uint32_t cyclesToSleep, uint64_t sleepingInterval = DEFAULT_SLEEPING_INTERVAL);
      bool isTimeToWakeUp();
      void goToSleep();
    private:
      uint32_t _cyclesSlept;
      uint32_t _cyclesToSleep;
      uint64_t _sleepingInterval;
      bool _wasResetFromSleepUp();
      void _updateCyclesSlept();
      void _readCyclesSlept();
      void _writeCyclesSlept();
  };
}
#endif
