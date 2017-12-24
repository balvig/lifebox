#include <Arduino.h>
#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer
#include "Led.h"

namespace Lifebox {
  // LEDC for ESP-32
  #define LEDC_TIMER_BITS 13
  #define LEDC_BASE_FREQ 5000
  
  #define OFF 0
  #define ON 1
  #define PULSE 2

  int Led::_count = 0;
  
  Led::Led(int pin)
  : _up_timer(), _on_timer(), _down_timer(), _off_timer() {
    _pin = pin;
    _setupPin();
  }
  
  void Led::setState(int newState) {
    switch(newState) {
      case ON:
        on();
        break;
      case OFF:
        off();
        break;
      case PULSE:
        _pulse();
        break;
    }
  }

  void Led::on(bool _stop_everything) { // default: true
    setBrightness(255, _stop_everything);
  }

  void Led::off(bool _stop_everything) { // default: true
    setBrightness(0, _stop_everything);
  }

  bool Led::isOn() {
    return getBrightness() == 255;
  }

  bool Led::isOff() {
    return getBrightness() == 0;
  }

  void Led::update() {
    if(_blinking) {
      _blink();
    }
    if(_fading) {
      _fade();
    }
  }

  void Led::setBrightness(int value, bool _stop_everything) {
    if(_stop_everything) {
      _stopEverything();
    }
    if(_pwm_value != value) {
      _pwm_value = constrain(value, 0, 255);
      _writePwm();
    }
  }

  void Led::setBrightnessPercent(int value, bool _stop_everything) {
    setBrightness(int(value / 100.0 * 255), _stop_everything);
  }

  int Led::getBrightness() {
    return _pwm_value;
  }

  int Led::getBrightnessPercent() {
    return int(getBrightness() / 255.0 * 100);
  }

  void Led::blink(unsigned long on_time, unsigned long off_time, int times) {
    _forever = false;
    _times   = times;

    _on_timer.setTimeout(on_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startBlinking();
  }

  // unlimited times
  void Led::blink(unsigned long on_time, unsigned long off_time) {
    _forever = true;
    _times   = 0;

    _on_timer.setTimeout(on_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startBlinking();
  }

  void Led::fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time, int times) {
    _forever = false;
    _times   = times;

    _up_timer.setTimeout(up_time);
    _on_timer.setTimeout(on_time);
    _down_timer.setTimeout(down_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startFading();
  }

  // unlimited times
  void Led::fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time) {
    _forever = true;
    _times   = 0;

    _up_timer.setTimeout(up_time);
    _on_timer.setTimeout(on_time);
    _down_timer.setTimeout(down_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startFading();
  }

  // Private

  void Led::_setupPin() {
    #ifdef ESP_PLATFORM
      _channel = _count;
      ledcSetup(_channel, LEDC_BASE_FREQ, LEDC_TIMER_BITS);
      ledcAttachPin(_pin, _channel);
       _count++;
    #else
      pinMode(_pin, OUTPUT);
    #endif
  }

  void Led::_writePwm() {
    #ifdef ESP_PLATFORM
      uint32_t valueMax = 255;
      uint32_t duty = (8191 / valueMax) * min(_pwm_value, valueMax);
      ledcWrite(_channel, duty);
    #else
      analogWrite(_pin, _pwm_value);
    #endif
  }

  void Led::_blink() {
    if(isOn() && _shouldBlinkOff()) {
      _blinkOff();
    }
    else if(isOff() && _shouldBlinkOn()) {
      _blinkOn();
    }
    else if(!isOn() && !isOff()) {
      _blinkOn();
    }
  }

  void Led::_blinkOff() {
    off(false); // don't stop everything
    _off_timer.restart();
    if(!_forever) {
      if(_times > 0) {_times--;}
      if(_times == 0) {_stopBlinking();}
    }
  }

  void Led::_blinkOn() {
    on(false); // don't stop everything
    _on_timer.restart();
  }

  bool Led::_shouldBlinkOff() {
    return _on_timer.isExpired();
  }

  bool Led::_shouldBlinkOn() {
    return _off_timer.isExpired();
  }
  
  void Led::_pulse() {
    int fadeInSpeed = random(1500, 2000);
    int fadeOutSpeed = fadeInSpeed - 500;
    fade(fadeInSpeed, 200, fadeOutSpeed, 250);
  }

  void Led::_fade() {
    switch(_state) {
      case _RISING:
        _rising();
        break;
      case _MAX:
        _onMax();
        break;
      case _FALLING:
        _falling();
        break;
      case _MIN:
        _onMin();
        break;
    }
  }

  void Led::_rising() {
    if(_shouldBeRising()) {
      setBrightness(_risingValue(), false); // don't stop everything
    }
    else {
      _on_timer.restart();
      _state = _MAX;
    }
  }

  bool Led::_shouldBeRising() {
    return _up_timer.isActive();
  }

  int Led::_risingValue() {
    return int(_up_timer.getPercentValue() / 100.0 * 255);
  }

  void Led::_onMax() {
    if(_shouldBeMax()) {
      if(!isOn()) {
        on(false); // don't stop everything
      }
    }
    else {
      _down_timer.restart();
      _state = _FALLING;
    }
  }

  bool Led::_shouldBeMax() {
    return _on_timer.isActive();
  }

  void Led::_falling() {
    if(_shouldBeFalling()) {
      setBrightness(_fallingValue(), false); // don't stop everything
    }
    else {
      _off_timer.restart();
      _state = _MIN;
    }
  }

  bool Led::_shouldBeFalling() {
    return _down_timer.isActive();
  }

  int Led::_fallingValue() {
    return int(_down_timer.getInversePercentValue() / 100.0 * 255);
  }

  void Led::_onMin() {
    if(_shouldBeMin()) {
      if(!isOff()) {
        off(false); // don't stop everything
      }
    }
    else {
      if(!_forever && _times > 0) {_times--;}
      if(!_forever && _times == 0) {
        _stopFading();
      }
      else {
        _up_timer.restart();
        _state = _RISING;
      }
    }
  }

  bool Led::_shouldBeMin() {
    return _off_timer.isActive();
  }

  void Led::_startFading() {
    if(_times > 0 || _forever) {
      _up_timer.restart();
      _state  = _RISING;
      _fading = true;
    }
  }

  void Led::_stopFading() {
    _fading = false;
  }

  void Led::_startBlinking() {
    if(_times > 0 || _forever) {
      _blinking = true;
    }
  }

  void Led::_stopBlinking() {
    _blinking = false;
  }

  void Led::_stopEverything() {
    _stopBlinking();
    _stopFading();
  }
}

