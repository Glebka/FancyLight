#pragma once

#include <stdint.h>

namespace brightness_control {
    extern bool adjustment_enabled;// = false;
    extern int8_t direction;// = -1;
    extern uint8_t intensity;// = 255;

    void handle();

    void startAdjust();
    void stopAdjust();
    void switchAdjustmentDirection();
}