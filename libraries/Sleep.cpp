#include "Sleep.h"

namespace Lifeboxes {
  const uint32_t RTC_SLEEP_COUNT_REGISTER = 65;
  
  Sleep::Sleep(uint32_t cyclesToSleep, uint64_t sleepingInterval) {
    Serial.println("Sleep interval" + sleepingInterval);
    _cyclesToSleep = cyclesToSleep;
    _sleepingInterval = sleepingInterval;
  }

  bool Sleep::isTimeToWakeUp() {
    _updateCyclesRemaining();

    if(_cyclesRemaining <= 0) {
      _resetCyclesRemaining();
      return true;
    }
    else {
      return false;
    }

  }

  void Sleep::goToSleep() {
    _writeCyclesRemaining();
    ESP.deepSleep(_sleepingInterval); 
  }

  // Private
  bool Sleep::_wasResetFromSleepUp() {
    const uint8_t reason = ESP.getResetInfoPtr()->reason;
    return reason == 6;
  }
  
  void Sleep::_updateCyclesRemaining() {
    if(_wasResetFromSleepUp()) {
      _resetCyclesRemaining();
    }
    else {
      _readCyclesRemaining();
      _cyclesRemaining--;
    }

    Serial.print("Sleep cycles remaining: ");
    Serial.println(_cyclesRemaining);
  }

  void Sleep::_resetCyclesRemaining() {
    _cyclesRemaining = _cyclesToSleep;
  }

  void Sleep::_readCyclesRemaining() {
    system_rtc_mem_read(RTC_SLEEP_COUNT_REGISTER, &_cyclesRemaining, sizeof(_cyclesRemaining));
  }

  void Sleep::_writeCyclesRemaining() {
    system_rtc_mem_write(RTC_SLEEP_COUNT_REGISTER, &_cyclesRemaining, sizeof(_cyclesRemaining));
  }
}
