/* Library aimed at doing a hardware test. Test to see the spark depending on dwell time. */


// Install libraries
#include <Arduino.h>


// Configuring Pins
const int ignition_pin = 8;
const int ignite_btn = 7;
const int dwell_pin = A0;


// Global variables

// Dwell
int dwell_pin_score;
int dwell_pin_value;
// dwell time for charging primary coil in milliseconds
int dwell;
const int min_dwell_time = 1;
const int max_dwell_time = 10;

// Ignite
bool ignite_btn_down = false;

// Observation delay in milliseconds
int observation_delay = 250;


void setup(){
    // Setup pins
    pinMode(ignition_pin, OUTPUT);
    pinMode(ignite_btn, INPUT_PULLUP);
}

void loop(){
    // Set dwell time
    int dwell_pin_value = analogRead(dwell_pin);
    if (dwell_pin_value != dwell_pin_score){
        // Set new dwell value
        dwell = map(dwell_pin_value, 0, 1023, min_dwell_time, max_dwell_time);
        // Update score
        dwell_pin_score = dwell_pin_value;
    };

    // Ignite when button pressed.
    if (!ignite_btn_down && digitalRead(ignite_btn)==LOW){
        // Set that button is pressed
        ignite_btn_down = true;
        // Start to charge primary coil
        digitalWrite(ignition_pin, HIGH);
        delayMicroseconds(dwell);
        // Stop charging primary coil for ignition
        digitalWrite(ignition_pin, LOW);
    };

    // Observation Delay (mostly not needed)
    delay(observation_delay);

    // Check that button is released to set as released.
    if (ignite_btn_down==true && digitalRead(ignite_btn)==HIGH){
        ignite_btn_down = false;
    }
}