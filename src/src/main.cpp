#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <Arduino.h>

#include "api.hpp"
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

  pinMode(LED_PIN, OUTPUT);
  pinMode(DCDC_EN_PIN, OUTPUT);
  pinMode(SENSE_PIN, INPUT);
  pinMode(TRIGGER_PIN, INPUT);

  // Soft PWM setup
  pinMode(PWM_PIN, OUTPUT);
  TIFR1 = (1 << TOV1);    // clear interrupt flag
  TIMSK1 = (1 << OCIE1A); // enable output compare match interrupt
  OCR1A = 0x00FF;         // set TOP to 255
  TCCR1B = (1 << CS10);   // start timer, no pre-scaler

  // Interrupts setup
  GIMSK = (1 << PCIE0);
  PCMSK0 = (1 << PCINT1); // enable pin change interrupt on PA1

  digitalWrite(DCDC_EN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(PWM_PIN, LOW);
  sei();
  delay(5);
  powerDown();
}

void handleEvent(event::Event event) {
    switch (fsm::currentState) {
        case fsm::OFF:
            if (event == event::BTN_PRESS) {
                turnOn();
                fsm::currentState = fsm::PRE_ON1;
            } else if (event == event::MAINS_OFF && true /* condition */) {
                turnOn();
                fsm::currentState = fsm::ON;
            }
            break;

        case fsm::PRE_ON1:
            if (event == event::BTN_LONG_PRESS) {
                brightness_control::startAdjust();
                fsm::currentState = fsm::BRIGHTNESS_ADJUSTMENT;
            } else if (event == event::BTN_RELEASE) {
                if (touch_control::isShortPress()) {
                    fsm::currentState = fsm::ON;
                }
            }
            break;
        case fsm::BRIGHTNESS_ADJUSTMENT:
            if (event == event::BTN_RELEASE) {
                brightness_control::stopAdjust();
                fsm::currentState = fsm::ON;
            } else if (event == event::BRIGHTNESS_LIMIT_REACHED) {
                brightness_control::stopAdjust();
                brightness_control::switchAdjustmentDirection();
                fsm::currentState = fsm::BRIGHTNESS_LIMIT;
            }
            break;
        case fsm::BRIGHTNESS_LIMIT:
            if (event == event::BTN_RELEASE) {
                fsm::currentState = fsm::ON;
            }
            break;
        case fsm::ON:
            if (event == event::BTN_PRESS) {
                fsm::currentState = fsm::PRE_OFF1;
            } else if (event == event::HIGH_ILLUMINATION && true /* condition */) {
                turnOff();
                fsm::currentState = fsm::OFF;
            }
            break;
        case fsm::PRE_OFF1:
            if (event == event::BTN_RELEASE) {
                // Check if the button release was less than 500 ms
                if (touch_control::isShortPress()) {
                    turnOff();
                    fsm::currentState = fsm::OFF;
                }
            } else if (event == event::BTN_LONG_PRESS) {
                brightness_control::startAdjust();
                fsm::currentState = fsm::BRIGHTNESS_ADJUSTMENT;
            }
            break;
        default:
            break;
    }
}

void loop()
{
  touch_control::handle();
  brightness_control::handle();
  handleEvent(event::currentEvent);
}

ISR(PCINT0_vect)
{
  if (digitalRead(TRIGGER_PIN) == HIGH)
  {
    touch_control::btn_press = millis();
    touch_control::btn_release = 0;
    event::currentEvent = event::BTN_PRESS;
  }
  else
  {
    if (touch_control::btn_press)
    {
      touch_control::btn_release = millis();
      event::currentEvent = event::BTN_RELEASE;
    }
  }
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

ISR(WDT_vect)
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}