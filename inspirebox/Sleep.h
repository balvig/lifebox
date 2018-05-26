#ifndef Lifeboxes_sleep_h
#define Lifeboxes_sleep_h

extern "C" {
#include "user_interface.h"
}
#include "Arduino.h"

namespace Lifeboxes  {
  const double SECONDS = 10 * micros();
  const double HOURS = 60 * 60 * SECONDS;
  const double DEFAULT_SLEEPING_INTERVAL = 1 * HOURS;

  class Sleep {
    public:
      Sleep(uint32_t hoursToSleep, uint32_t sleepingInterval = DEFAULT_SLEEPING_INTERVAL);
      bool isTimeToWakeUp();
      void goToSleep();
    private:
      uint32_t _hoursSlept;
      uint32_t _hoursToSleep;
      uint32_t _sleepingInterval;
      bool _wasResetFromSleepUp();
      void _updateHoursSlept();
      void _readHoursSlept();
      void _writeHoursSlept();
  };
}
#endif
