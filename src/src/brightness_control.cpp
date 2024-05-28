#include <Arduino.h>

#include "common.hpp"
#include "brightness_control.hpp"

namespace brightness_control
{
    bool adjustment_enabled = false;
    int8_t direction = -1;
    uint8_t intensity = 255;

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