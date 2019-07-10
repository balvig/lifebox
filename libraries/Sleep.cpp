#include "Sleep.h"

namespace Lifeboxes {
  const uint32_t RTC_SLEEP_COUNT_REGISTER = 65;
  const uint32_t UPPER_LIMIT = 8760; // Arduino randomly decided to sleep for 2809417741 hours
  
  Sleep::Sleep(uint32_t cyclesToSleep, uint64_t sleepingInterval) {
    _defaultCyclesToSleep = cyclesToSleep;
    _sleepingInterval = sleepingInterval;
  }

  bool Sleep::isTimeToWakeUp() {
    _updateCyclesRemaining();

    if(cyclesRemaining <= 0 || cyclesRemaining > UPPER_LIMIT) {
      resetCyclesRemaining(_defaultCyclesToSleep);
      return true;
    }
    else {
      return false;
    }
  }

  void Sleep::resetCyclesRemaining(uint32_t newValue) {
    cyclesRemaining = newValue;
  }

  void Sleep::goToSleep() {
    Serial.println("writing cycles");
    _writeCyclesRemaining();
    Serial.println("Sleeping");
    ESP.deepSleep(_sleepingInterval); 
  }

  // Private
  bool Sleep::_wasResetFromSleepUp() {
    const uint8_t reason = ESP.getResetInfoPtr()->reason;
    return reason == 6;
  }
  
  void Sleep::_updateCyclesRemaining() {
    if(_wasResetFromSleepUp()) {
      resetCyclesRemaining(_defaultCyclesToSleep);
    }
    else {
      _readCyclesRemaining();
      cyclesRemaining--;
    }
  }

  void Sleep::_readCyclesRemaining() {
    system_rtc_mem_read(RTC_SLEEP_COUNT_REGISTER, &cyclesRemaining, sizeof(cyclesRemaining));
  }

  void Sleep::_writeCyclesRemaining() {
    system_rtc_mem_write(RTC_SLEEP_COUNT_REGISTER, &cyclesRemaining, sizeof(cyclesRemaining));
  }
}
