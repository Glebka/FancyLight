#pragma once

#include <Arduino.h>

#define LED_PIN 6
#define SENSE_PIN 1
#define DCDC_EN_PIN 10
#define PWM_PIN 8
#define TRIGGER_PIN 9

namespace brightness_control {
    extern bool adjustment_enabled;// = false;
    extern int8_t direction;// = -1;
    extern uint8_t intensity;// = 255;

    void handle();

    void startAdjust();
    void stopAdjust();
    void switchAdjustmentDirection();
}

namespace touch_control {
    extern unsigned long btn_press;// = 0;
    extern unsigned long btn_release;// = 0;
    extern unsigned long btn_longpress;// = 0;
    extern bool last_press_short;// = false;

    void handle();
    bool isShortPress();

}

void turnOn();
void turnOff();
void idle();
void powerDown();