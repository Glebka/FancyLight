#include <avr/power.h>
#include <avr/sleep.h>
#include <Arduino.h>

#include "common.hpp"
#include "config.hpp"
#include "power_control.hpp"
#include "touch_control.hpp"
#include "illumination_control.hpp"
#include "brightness_control.hpp"
#include "fsm.hpp"

void setup()
{
  cli();

  // disable ADC
  ADCSRA = 0;

  // turn off various modules
  power_all_disable();

  // but enable the necessary ones
  power_timer0_enable();
  power_timer1_enable();

  config::init();
  config::handle();

  power_control::init();
  touch_control::init();
  illumination_control::init();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  sei();
  delay(5);
  power_control::powerDown();
}

void loop()
{
  config::handle();
  touch_control::handle();
  brightness_control::handle();
  illumination_control::handle();
  fsm::handleEvent(event::currentEvent);
}

// TODO: get rid of this by fixing schematic and using hardware PWM
ISR(TIMER1_COMPA_vect)
{
  static uint8_t softcount = 0xFF;
  if (fsm::currentState == fsm::OFF)
  {
    return;
  }
  softcount++;
  if (softcount == 0)
  {
    digitalWrite(PWM_PIN, HIGH);
  }
  if (softcount == brightness_control::intensity)
  {
    digitalWrite(PWM_PIN, LOW);
  }
  TCNT1 = 0;
}
