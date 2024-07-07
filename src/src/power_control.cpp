#include <Arduino.h>
#include <avr/sleep.h>

#include "common.hpp"
#include "illumination_control.hpp"
#include "power_control.hpp"
#include "brightness_control.hpp"

namespace power_control
{
    void init()
    {
        pinMode(DCDC_EN_PIN, OUTPUT);
        pinMode(PWM_PIN, OUTPUT);
        pinMode(SENSE_PIN, INPUT);
        digitalWrite(DCDC_EN_PIN, LOW);
        digitalWrite(PWM_PIN, LOW);
    }

    void turnLightOn()
    {
        digitalWrite(DCDC_EN_PIN, HIGH);
        analogWrite(PWM_PIN, brightness_control::intensity);
    }

    void turnLightOff()
    {
        digitalWrite(DCDC_EN_PIN, LOW);
        digitalWrite(PWM_PIN, LOW);
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
}