#include <Arduino.h>
#include "common.hpp"
#include "touch_control.hpp"

namespace touch_control
{
    unsigned long btn_press = 0;
    unsigned long btn_release = 0;
    unsigned long btn_longpress = 0;
    bool last_press_short = false;

    void init()
    {
        pinMode(TRIGGER_PIN, INPUT);
        // Interrupts setup
        GIMSK |= (1 << PCIE0);
        PCMSK0 |= (1 << PCINT1); // enable pin change interrupt on PA1
    }

    void handle()
    {
        if (btn_press && !btn_release && (millis() - btn_press) > 500 && !btn_longpress)
        {
            // trigger long press event
            btn_longpress = true;
            last_press_short = false;
            event::currentEvent = event::BTN_LONG_PRESS;
        }
        if (btn_press && btn_release)
        {
            if ((btn_release - btn_press) < 500)
            {
                last_press_short = true;
            }
            btn_press = 0;
            btn_release = 0;
            btn_longpress = false;
        }
    }
    bool isShortPress()
    {
        return last_press_short;
    }

}

ISR(PCINT0_vect)
{
  if (digitalRead(TRIGGER_PIN) == HIGH)
  {
    touch_control::btn_press = millis();
    touch_control::btn_release = 0;
    event::currentEvent = event::BTN_PRESS;
    return;
  }
  if (digitalRead(TRIGGER_PIN) == LOW && touch_control::btn_press != 0)
  {
    touch_control::btn_release = millis();
    event::currentEvent = event::BTN_RELEASE;
  }
}