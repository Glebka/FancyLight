#pragma once

#include "common.hpp"
namespace fsm {
    extern State currentState;
    
    void handleEvent(event::Event event);
}