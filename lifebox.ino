#include <RBD_Light.h>
#include <RBD_Timer.h>
#include <RBD_Button.h>

class Led {
  #define OFF 0
  #define ON 1
  #define PULSE 2

  int state = OFF;
  RBD::Light *light;

  public:
  Led(int pin) {
    light = new RBD::Light(pin);
  }

  void setState(int newState) {
    switch(newState) {
      case ON:
        on();
        break;
      case OFF:
        off();
        break;
      case PULSE:
        pulse();
        break;
    }
  }

  void update() {
    light->update();
  }

  private:
  void pulse() {
    if(state != PULSE) {
      int fadeInSpeed = random(1500, 2000);
      int fadeOutSpeed = fadeInSpeed - 500;
      light->fade(fadeInSpeed, 200, fadeOutSpeed, 250);
      state = PULSE;
    }
  }

  void on() {
    light->on();
    state = ON;
  }

  void off() {
    light->off();
    state = OFF;
  }
};


RBD::Button button(2);
RBD::Timer timer;
Led leds[] = { 5, 6, 7, 8, 9, 10, 11 };
int ledsCount = 7;

void updateLights() {
  for (int i = 0; i < ledsCount; i++) {
    leds[i].update();
  }
}

void updateState() {
  // int results[] = { OFF, OFF, ON, OFF, OFF, PULSE, PULSE };
  int results[] = { random(2), random(2), random(2), random(2), random(3), random(3), random(3) };
  int resultsCount = 7;
  for (int i = 0; i < resultsCount; i++) {
    int state = results[i];
    leds[i].setState(state);
  }
}

void setup() {
  timer.setTimeout(10000);
  timer.restart();
  updateState();
}

void loop() {
  //if(timer.onRestart()) {
  if(button.onPressed()) {
    updateState();
  }
  updateLights();
}
