#ifndef Lifeboxes_sleep_h
#define Lifeboxes_sleep_h

extern "C" {
#include "user_interface.h"
}
#include "Arduino.h"

namespace Lifeboxes  {
  class Sleep {
    public:
      Sleep(uint32_t hoursToSleep);
      bool isTimeToWakeUp();
      void goToSleep();
    private:
      uint32_t _hoursSlept;
      uint32_t _hoursToSleep;
      bool _wasResetFromSleepUp();
      void _updateHoursSlept();
      void _readHoursSlept();
      void _writeHoursSlept();
  };
}
#endif
