#pragma once

namespace touch_control {
    extern unsigned long btn_press;// = 0;
    extern unsigned long btn_release;// = 0;
    extern unsigned long btn_longpress;// = 0;
    extern bool last_press_short;// = false;

    void init();
    void handle();
    bool isShortPress();
}