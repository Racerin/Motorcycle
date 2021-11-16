// Library aimed at doing a hardware test. Test to see the spark depending on dwell time.

// Install libraries
#include <Arduino.h>

// Configuring Pins
int ignition_pin = 8;
int dwell_pin = A0;

// Global variables
int dwell_pin_score;
int dwell_pin_value;
// Dwell time for charging in milliseconds
int dwell;
int dwell_min=1;
int dwell_max=10;

void setup(){
    // Setup pins
    pinMode(ignition_pin, OUTPUT);
    // pinMode(dwell_pin, )
}

void loop(){
    // Set dwell time
    int dwell_pin_value = analogRead(dwell_pin)
    if (dwell_pin_value != dwell_pin_score){
        // Set new dwell value
        dwell = map(dwell_pin_value, 0, 1023, dwell_min, dwell_max);
    };
}