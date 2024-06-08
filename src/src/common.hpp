#pragma once

// counter-clockwise pinout
#define LED_PIN 6
#define SENSE_PIN 1
#define DCDC_EN_PIN 10
#define PWM_PIN 8
#define TRIGGER_PIN 9
#define PHOTOR_SINK_PIN 2
#define ADC_PIN 7
#define ADC_ANALOG_PIN A3
#define MODE1_PIN 4
#define MODE2_PIN 0
#define LOW_ILLUMINATION_THRESHOLD 15
#define HYSTERESIS 160

namespace event {
    // Define events
    typedef enum
    {
        BTN_PRESS,
        BTN_RELEASE,
        BTN_LONG_PRESS,
        MAINS_ON,
        MAINS_OFF,
        LOW_ILLUMINATION,
        HIGH_ILLUMINATION,
        WDT_INTERRUPT,
        RF_PACKET_RECEIVED,
        RF_TIMEOUT,
        BRIGHTNESS_LIMIT_REACHED,
        NUM_EVENTS
    } Event;

    extern Event currentEvent;
}

namespace fsm
{
    typedef enum
    {
        OFF,
        PRE_ON1,
        BRIGHTNESS_ADJUSTMENT,
        BRIGHTNESS_LIMIT,
        ON,
        PRE_OFF1,
        NUM_STATES
    } State;
}