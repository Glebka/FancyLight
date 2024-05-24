#include <avr/power.h>
#include <avr/sleep.h>

#include "api.hpp"
#include "fsm.hpp"

bool brightness_control::adjustment_enabled = false;
int8_t brightness_control::direction = -1;
uint8_t brightness_control::intensity = 255;

unsigned long touch_control::btn_press = 0;
unsigned long touch_control::btn_release = 0;
unsigned long touch_control::btn_longpress = 0;
bool touch_control::last_press_short = false;

void turnOn()
{
    digitalWrite(DCDC_EN_PIN, HIGH);
}

void turnOff()
{
    digitalWrite(DCDC_EN_PIN, LOW);
    digitalWrite(PWM_PIN, LOW);
    powerDown();
}

void powerDown()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
}

void idle()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
}

namespace brightness_control
{
    void handle()
    {
        if (adjustment_enabled)
        {
            cli();
            if (direction > 0)
            {
                intensity = min(intensity + direction, 255);
            }
            if (direction < 0)
            {
                intensity = max(intensity + direction, 1);
            }
            if (direction > 0 && intensity == 255)
            {
                event::currentEvent = event::BRIGHTNESS_LIMIT_REACHED;
            }
            if (direction < 0 && intensity == 1)
            {
                event::currentEvent = event::BRIGHTNESS_LIMIT_REACHED;
            }
            sei();
            delay(10);
        }
    }
    void startAdjust()
    {
        adjustment_enabled = true;
    }

    void stopAdjust()
    {
        adjustment_enabled = false;
    }

    void switchAdjustmentDirection()
    {
        direction = -direction;
    }
}

namespace touch_control
{
    void handle()
    {
        if (btn_press && !btn_release && (millis() - btn_press) > 500 && !btn_longpress)
        {
            // trigger long press event
            btn_longpress = true;
            last_press_short = false;
            event::currentEvent = event::BTN_LONG_PRESS;
        }
        if (btn_press && btn_release) {
            if ((btn_release - btn_press) < 500) {
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