/* Code for arduino to regulate output voltage. 
Different approach to 'main'.
Get threshold as a 10bit vlue from the start and compare analogRead to 10bit threshold value.
*/

#include <Arduino.h>

// Configure Pins
const int voltage_sensor_pin = A1;
const int output_analog_pin = 5;

// Electronics values
// V,input / V,pin = R,input / R,pin
const int r_pin = 10000;
const int r_besides_pin = 20000;
const int r_total = r_pin + r_besides_pin;
const float r_ratio = r_total/r_pin;    //      R,input / R,pin

const float voltage_threshold = 14.1;
int input_threshold_value;

int input_analog_value;


int setup_voltage_bit_threshold(){
    // Working constant
    float constant = 1;

    // Convert threshold voltage to pin voltage
    constant *= voltage_threshold/r_ratio;

    // Convert pin voltage to 10bits
    constant = map(constant, 0, 5, 0, 1023);

    // Return int value for analog read threshold
    return int(constant);
};

void adjust_output_pin(){
    // Get analogRead value
    input_analog_value = analogRead(voltage_sensor_pin);
    if(input_analog_value > input_threshold_value){
        digitalWrite(output_analog_pin, LOW);
    }
    else {
        digitalWrite(output_analog_pin, HIGH);
    }
};

void setup(){
    input_threshold_value = setup_voltage_bit_threshold();
};


void loop(){
    adjust_output_pin();
};