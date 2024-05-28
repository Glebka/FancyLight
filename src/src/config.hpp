#pragma once

#include <stdint.h>

namespace config
{
    enum Feature
    {
        ILLUMINATION_DETECTION = 1,
        MAINS_OUTAGE_DETECTION_USB = 2,
        BT_REMOTE_CONTROL = 4,
    };

    extern uint8_t feature_set;

    void init();
    void handle();
    bool hasFeature(Feature feature);
}
