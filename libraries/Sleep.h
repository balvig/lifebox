#ifndef Lifeboxes_sleep_h
#define Lifeboxes_sleep_h

extern "C" {
#include "user_interface.h"
}
#include "Arduino.h"

namespace Lifeboxes  {
  const double SECONDS = 10 * micros();
  const double HOURS = 60 * 60 * SECONDS;
  const double DEFAULT_SLEEPING_INTERVAL = ESP.deepSleepMax();

  class Sleep {
    public:
      Sleep(uint32_t cyclesToSleep, uint32_t sleepingInterval = DEFAULT_SLEEPING_INTERVAL);
      bool isTimeToWakeUp();
      void goToSleep();
    private:
      uint32_t _cyclesSlept;
      uint32_t _cyclesToSleep;
      uint32_t _sleepingInterval;
      bool _wasResetFromSleepUp();
      void _updateCyclesSlept();
      void _readCyclesSlept();
      void _writeCyclesSlept();
  };
}
#endif
