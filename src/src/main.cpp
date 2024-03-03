#include <avr/power.h>
#include <avr/sleep.h>
#include <Arduino.h>
#include <EEPROM.h>

#define LED_PIN 6
#define SENSE_PIN 1
#define DCDC_EN_PIN 10
#define PWM_PIN 8
#define TRIGGER_PIN 9

enum State {
  OFF,
  ON,
  ON_HOLDING,
};

// Soft PWM setup
volatile uint8_t intensity = 10;
volatile int8_t direction = 1;
volatile uint8_t state = 0;
volatile unsigned long btn_press = 0;
volatile unsigned long btn_release = 0;

void power_down()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
}

void idle()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
}

void setup() {
  cli();

  // disable ADC
  ADCSRA = 0;

  // turn off various modules
  power_all_disable();

  // but enable the necessary ones
  power_timer0_enable();
  power_timer1_enable();


  pinMode(LED_PIN, OUTPUT);
  pinMode(DCDC_EN_PIN, OUTPUT);
  pinMode(SENSE_PIN, INPUT);
  pinMode(TRIGGER_PIN, INPUT);

  // Soft PWM setup
  pinMode(PWM_PIN, OUTPUT);
  TIFR1 = (1 << TOV1);  // clear interrupt flag
  TIMSK1 = (1 << OCIE1A); // enable output compare match interrupt
  OCR1A = 0x00FF; // set TOP to 255
  TCCR1B = (1 << CS10);  // start timer, no pre-scaler


  // Interrupts setup
  GIMSK = (1 << PCIE0);
  PCMSK0 = (1 << PCINT1); // enable pin change interrupt on PA1

  digitalWrite(DCDC_EN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(PWM_PIN, LOW);

  sei();
  delay(5);
  power_down();
}

void loop() {
  if (btn_press && !btn_release && state == OFF) {
    state = ON_HOLDING;
    digitalWrite(DCDC_EN_PIN, HIGH);
  }
  if (btn_press && !btn_release && (millis() - btn_press) > 500) {
    if (intensity + direction >= 255) {
      direction = -1;
    }
    if (intensity + direction <= 1 ) {
      direction = 1;
    }
    intensity += direction;
    delay(10);
  }
  if (btn_press && btn_release && (btn_release - btn_press) < 500 && state != ON_HOLDING) {
    btn_press = 0;
    btn_release = 0;
    state = OFF;
    digitalWrite(PWM_PIN, LOW);
    digitalWrite(DCDC_EN_PIN, LOW);
    power_down();
  }
  if (btn_press && btn_release && (state == ON_HOLDING || state == ON)) {
    btn_press = 0;
    btn_release = 0;
    state = ON;
    idle();
  }
}

ISR(PCINT0_vect)
{
  if (digitalRead(TRIGGER_PIN) == HIGH) {
    btn_press = millis();
  } else {
    btn_release = millis();
  }
}

// TODO: get rid of this by fixing schematic and using hardware PWM
ISR(TIMER1_COMPA_vect)
{
  static uint8_t softcount = 0xFF;
  if (state == OFF) {
    return;
  }
  softcount++;
  if (softcount == 0) {
    digitalWrite(PWM_PIN, HIGH);
  }
  if (softcount == intensity) {
    digitalWrite(PWM_PIN, LOW);
  }
  TCNT1 = 0;
}
