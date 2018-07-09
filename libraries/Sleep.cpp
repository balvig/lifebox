#include "Sleep.h"

namespace Lifeboxes {
  const uint32_t RTC_SLEEP_COUNT_REGISTER = 65;
  
  Sleep::Sleep(uint32_t hoursToSleep, uint32_t sleepingInterval) {
    _hoursToSleep = hoursToSleep;
    _sleepingInterval = sleepingInterval;
  }

  bool Sleep::isTimeToWakeUp() {
    _updateHoursSlept();

    if(_hoursSlept >= _hoursToSleep) {
      _hoursSlept = 0;
    }

    return _hoursSlept == 0;
  }

  void Sleep::goToSleep() {
    _writeHoursSlept();
    ESP.deepSleep(_sleepingInterval); 
  }

  // Private
  bool Sleep::_wasResetFromSleepUp() {
    const uint8_t reason = ESP.getResetInfoPtr()->reason;
    return reason == 6;
  }
  
  void Sleep::_updateHoursSlept() {
    if(_wasResetFromSleepUp()) {
      _hoursSlept = 0;
    }
    else {
      _readHoursSlept();
      _hoursSlept++;
    }
  }

  void Sleep::_readHoursSlept() {
    system_rtc_mem_read(RTC_SLEEP_COUNT_REGISTER, &_hoursSlept, sizeof(_hoursSlept));
  }

  void Sleep::_writeHoursSlept() {
    system_rtc_mem_write(RTC_SLEEP_COUNT_REGISTER, &_hoursSlept, sizeof(_hoursSlept));
  }
}
