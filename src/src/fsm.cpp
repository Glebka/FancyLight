#include "fsm.hpp"

fsm::State fsm::currentState = fsm::OFF;

event::Event event::currentEvent = event::NUM_EVENTS;