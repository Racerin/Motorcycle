/* Code for arduino to regulate output voltage. */

#include <Arduino.h>

// Configure Pins
const int VOLTAGE_SENSOR_PIN = A1;
const int output_analog_pin = 5;

// IO
int output_option = 0;
int analog_input;
int analog_write_counter = 0;
int temp_analog_write_counter = analog_write_counter;


// Electronics values
// V,input / V,pin = R,input / R,pin
const int R_PIN = 10000;
const int R_BESIDES_PIN = 20000;
const int R_TOTAL = R_PIN + R_BESIDES_PIN;
const float R_RATIO = R_TOTAL/R_PIN;    //      R,input / R,pin

const float VOLTAGE_THRESHOLD = 14.1;

float pin_voltage;
float input_voltage;

int get_input_voltage(){
    // Read input pin
    analog_input = analogRead(VOLTAGE_SENSOR_PIN);
    
    // Convert to input voltage
    pin_voltage = map(analog_input, 0, 1023, 0, 5);

    // Extrapolate to read input voltage, (Vin = R,ratio * V,pin)
    return pin_voltage * R_RATIO;
};

void adjust_output_pin(){
    switch(output_option){
        case 0:
            // Turn ON/OFF the output pin based on input_voltage to threshold
            if(input_voltage > VOLTAGE_THRESHOLD){
                digitalWrite(output_analog_pin, LOW);
                }
            else{
                digitalWrite(output_analog_pin, HIGH);
                }
            break;

        default:
            // Use builtin analogWrite. Increment analog write value up and down accordingly.
            if(input_voltage > VOLTAGE_THRESHOLD){
                temp_analog_write_counter -= 1;
            }
            else{
                temp_analog_write_counter += 1;
            }
            // Ensure analog_write_counter within input range
            temp_analog_write_counter = constrain(temp_analog_write_counter, 0, 255);
            if (temp_analog_write_counter != analog_write_counter){
                // Update analog_write_counter
                temp_analog_write_counter = analog_write_counter;
                // Set new analog/PWM write value
                analogWrite(output_analog_pin, analog_write_counter);
            };
            break;
    }
};

void setup(){
    // Configure pins
    // pinMode(output_analog_pin, OUTPUT);
};

void loop(){
    // Read/Get input voltage
    input_voltage = get_input_voltage();

    // Adjust output pin's PWM according to voltage
    adjust_output_pin();
};