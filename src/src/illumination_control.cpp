#include <avr/power.h>
#include <avr/wdt.h>
#include <Arduino.h>

#include "illumination_control.hpp"
#include "config.hpp"

inline void wdtEnable()
{
  wdt_enable(WDTO_4S);
  WDTCSR |= (1 << WDIE);
}

inline void adc_enable()
{
  digitalWrite(PHOTOR_SINK_PIN, LOW);
  power_adc_enable();
  ADMUX = (1 << MUX0) | (1 << MUX1); // select ADC3 (PA3) as input
  // enable ADC
  ADCSRA = (1 << ADEN)  // enable ADC
    | (1 << ADIE)       // enable ADC interrupt
    | (1 << ADPS1) | (1 << ADPS0)      // set prescaler to 16 (clock speed is 8 MHz / 16)
    ;
  DIDR0 = (1 << ADC3D); // disable digital input on ADC3
}

inline void adc_start()
{
  ADCSRA |= (1 << ADSC);
}

inline void adc_disable()
{
  ADCSRA = 0;
  power_adc_disable();
  digitalWrite(PHOTOR_SINK_PIN, HIGH);
}

namespace illumination_control
{

    uint16_t adc_value = 0;
    uint16_t high_illumination_threshold = 1000;
    event::Event last_event = event::NUM_EVENTS;
    unsigned long last_measurement_time = 0;

    void init()
    {
        if (config::hasFeature(config::ILLUMINATION_DETECTION))
        {
            pinMode(ADC_PIN, INPUT);
            pinMode(PHOTOR_SINK_PIN, OUTPUT);
            digitalWrite(PHOTOR_SINK_PIN, HIGH);
            wdtEnable();
        } else {
            pinMode(PHOTOR_SINK_PIN, OUTPUT);
            digitalWrite(PHOTOR_SINK_PIN, HIGH);
            pinMode(ADC_PIN, OUTPUT);
            digitalWrite(ADC_PIN, HIGH);
            wdt_disable();
        }
    }

    void update_high_illumination_threshold()
    {
        adc_enable();
        high_illumination_threshold = min(800, analogRead(ADC_ANALOG_PIN) + HYSTERESIS);
        adc_disable();
    }

    void reset()
    {
        last_measurement_time = 0;
        last_event = event::NUM_EVENTS;
        high_illumination_threshold = 800;
    }

    void handle()
    {
        if (last_measurement_time)
        {
            last_measurement_time = 0;
            if (adc_value < LOW_ILLUMINATION_THRESHOLD && last_event != event::LOW_ILLUMINATION)
            {
                event::currentEvent = event::LOW_ILLUMINATION;
                last_event = event::LOW_ILLUMINATION;
            }
            else if (adc_value > high_illumination_threshold && last_event != event::HIGH_ILLUMINATION)
            {
                event::currentEvent = event::HIGH_ILLUMINATION;
                last_event = event::HIGH_ILLUMINATION;
            } else {
                last_event = event::NUM_EVENTS;
            }
        }
    }
}

ISR(WDT_vect)
{
  WDTCSR |= (1 << WDIE);
  adc_enable();
  adc_start();
  digitalWrite(LED_PIN, HIGH);
}

ISR(ADC_vect)
{
  static uint8_t low = 0;
  static uint8_t high = 0;
  low = ADCL;
  high = ADCH;
  illumination_control::adc_value = (high << 8) | low;
  illumination_control::last_measurement_time = millis();
  adc_disable();
  digitalWrite(LED_PIN, LOW);
}