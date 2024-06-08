#include <Arduino.h>
#include "power_control.hpp"
#include "illumination_control.hpp"
#include "touch_control.hpp"
#include "brightness_control.hpp"
#include "config.hpp"
#include "fsm.hpp"

event::Event event::currentEvent = event::NUM_EVENTS;
namespace fsm
{
    State currentState = fsm::OFF;

    void handleEvent(event::Event event)
    {
        switch (fsm::currentState)
        {
        case fsm::OFF:
            if (event == event::BTN_PRESS)
            {
                power_control::turnLightOn();
                illumination_control::reset();
                fsm::currentState = fsm::PRE_ON1;
            }
            else if (event == event::MAINS_OFF && config::hasFeature(config::MAINS_OUTAGE_DETECTION_USB))
            {
                power_control::turnLightOn();
                fsm::currentState = fsm::ON;
            }
            else if (event == event::LOW_ILLUMINATION && config::hasFeature(config::ILLUMINATION_DETECTION))
            {
                power_control::turnLightOn();
                fsm::currentState = fsm::ON;
                delay(10);
                illumination_control::update_high_illumination_threshold();
            }
            break;

        case fsm::PRE_ON1:
            if (event == event::BTN_LONG_PRESS)
            {
                brightness_control::startAdjust();
                fsm::currentState = fsm::BRIGHTNESS_ADJUSTMENT;
            }
            else if (event == event::BTN_RELEASE)
            {
                if (touch_control::isShortPress())
                {
                    fsm::currentState = fsm::ON;
                }
            }
            break;
        case fsm::BRIGHTNESS_ADJUSTMENT:
            if (event == event::BTN_RELEASE)
            {
                brightness_control::stopAdjust();
                fsm::currentState = fsm::ON;
            }
            else if (event == event::BRIGHTNESS_LIMIT_REACHED)
            {
                brightness_control::stopAdjust();
                brightness_control::switchAdjustmentDirection();
                fsm::currentState = fsm::BRIGHTNESS_LIMIT;
            }
            break;
        case fsm::BRIGHTNESS_LIMIT:
            if (event == event::BTN_RELEASE)
            {
                fsm::currentState = fsm::ON;
            }
            break;
        case fsm::ON:
            if (event == event::BTN_PRESS)
            {
                fsm::currentState = fsm::PRE_OFF1;
            }
            else if (event == event::HIGH_ILLUMINATION && config::hasFeature(config::ILLUMINATION_DETECTION))
            {
                power_control::turnLightOff();
                fsm::currentState = fsm::OFF;
            }
            break;
        case fsm::PRE_OFF1:
            if (event == event::BTN_RELEASE)
            {
                // Check if the button release was less than 500 ms
                if (touch_control::isShortPress())
                {
                    power_control::turnLightOff();
                    fsm::currentState = fsm::OFF;
                }
            }
            else if (event == event::BTN_LONG_PRESS)
            {
                brightness_control::startAdjust();
                fsm::currentState = fsm::BRIGHTNESS_ADJUSTMENT;
            }
            break;
        default:
            break;
        }
        //event::currentEvent = event::NUM_EVENTS;
        if (fsm::currentState == fsm::OFF)
        {
            power_control::powerDown();
        }
    }
}