#include "Sleep.h"

namespace Lifeboxes {
  const uint32_t RTC_SLEEP_COUNT_REGISTER = 65;
  
  Sleep::Sleep(uint32_t cyclesToSleep, uint32_t sleepingInterval) {
    _cyclesToSleep = cyclesToSleep;
    _sleepingInterval = sleepingInterval;
  }

  bool Sleep::isTimeToWakeUp() {
    _updateCyclesSlept();

    if(_cyclesSlept >= _cyclesToSleep) {
      _cyclesSlept = 0;
    }

    return _cyclesSlept == 0;
  }

  void Sleep::goToSleep() {
    _writeCyclesSlept();
    ESP.deepSleep(_sleepingInterval); 
  }

  // Private
  bool Sleep::_wasResetFromSleepUp() {
    const uint8_t reason = ESP.getResetInfoPtr()->reason;
    return reason == 6;
  }
  
  void Sleep::_updateCyclesSlept() {
    if(_wasResetFromSleepUp()) {
      _cyclesSlept = 0;
    }
    else {
      _readCyclesSlept();
      _cyclesSlept++;
    }
  }

  void Sleep::_readCyclesSlept() {
    system_rtc_mem_read(RTC_SLEEP_COUNT_REGISTER, &_cyclesSlept, sizeof(_cyclesSlept));
  }

  void Sleep::_writeCyclesSlept() {
    system_rtc_mem_write(RTC_SLEEP_COUNT_REGISTER, &_cyclesSlept, sizeof(_cyclesSlept));
  }
}
