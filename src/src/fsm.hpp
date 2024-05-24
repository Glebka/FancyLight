#pragma once

#include <stdbool.h>
#include <stdint.h>

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

    // Define function pointers for enter, exit, and action handlers
    typedef void (*ActionHandler)(void);
    typedef bool (*PredicateHandler)(void);

    // Define a structure for state transitions
    typedef struct
    {
        State nextState;
        ActionHandler enterHandler;
        ActionHandler exitHandler;
        PredicateHandler predicate;
    } StateTransition;

    extern State currentState;
}

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