#include <Arduino.h>

#include "common.hpp"
#include "config.hpp"
#include "illumination_control.hpp"
#include "power_control.hpp"


namespace config {
    uint8_t feature_set = 0;

    void init() {
        pinMode(MODE1_PIN, OUTPUT);
        pinMode(MODE2_PIN, OUTPUT);
        digitalWrite(MODE1_PIN, LOW);
        digitalWrite(MODE2_PIN, LOW);
    }

    bool hasFeature(Feature feature) {
        return feature_set & feature;
    }

    void handle() {
        static bool mode1 = false;
        static bool mode2 = false;
        static uint8_t last_feature_set = 0;

        last_feature_set = feature_set;
        feature_set = 0;

        pinMode(MODE1_PIN, INPUT_PULLUP);
        pinMode(MODE2_PIN, INPUT_PULLUP);

        mode1 = !digitalRead(MODE1_PIN);
        mode2 = !digitalRead(MODE2_PIN);

        pinMode(MODE1_PIN, OUTPUT);
        pinMode(MODE2_PIN, OUTPUT);
        digitalWrite(MODE1_PIN, LOW);
        digitalWrite(MODE2_PIN, LOW);

        if (mode1) {
            feature_set |= ILLUMINATION_DETECTION;
        }
        if (mode2) {
            feature_set |= ILLUMINATION_DETECTION | MAINS_OUTAGE_DETECTION_USB;
        }
        if (mode1 && mode2) {
            feature_set |= BT_REMOTE_CONTROL;
        }
        if (feature_set != last_feature_set) {
            // re-init modules
            illumination_control::init();
            power_control::init();
        }
    }
}