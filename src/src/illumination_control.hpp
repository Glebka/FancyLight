#pragma once

#include <stdint.h>
#include <common.hpp>

namespace illumination_control {
    
    extern uint16_t adc_value;
    extern uint16_t high_illumination_threshold;
    extern event::Event last_event;
    extern unsigned long last_measurement_time;

    void init();
    void handle();
}