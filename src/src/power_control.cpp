#include <Arduino.h>
#include <avr/sleep.h>

#include "common.hpp"
#include "illumination_control.hpp"
#include "power_control.hpp"

namespace power_control
{
    void init()
    {
        pinMode(DCDC_EN_PIN, OUTPUT);
        pinMode(PWM_PIN, OUTPUT);
        pinMode(SENSE_PIN, INPUT);
        digitalWrite(DCDC_EN_PIN, LOW);
        digitalWrite(PWM_PIN, LOW);

        // Soft PWM setup
        TIFR1 = (1 << TOV1);    // clear interrupt flag
        TIMSK1 = (1 << OCIE1A); // enable output compare match interrupt
        OCR1A = 0x00FF;         // set TOP to 255
    }

    void turnLightOn()
    {
        digitalWrite(DCDC_EN_PIN, HIGH);
        TCCR1B = (1 << CS10);   // start timer, no pre-scaler
    }

    void turnLightOff()
    {
        digitalWrite(DCDC_EN_PIN, LOW);
        digitalWrite(PWM_PIN, LOW);
        TCCR1B = 0; // stop timer
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