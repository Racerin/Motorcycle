/* Code for arduino to regulate output voltage. 
Different approach to 'main'.
Get threshold as a 10bit vlue from the start and compare analogRead to 10bit threshold value.
*/

#include <Arduino.h>

class PinInValues{
    private:
        int __inputs [10];
        int __count = 0;
        int __total = 0;

    public:
        PinInValues(int size){
            int __inputs[size] = {0};
        }

        int size(){
            return sizeof(__inputs)/sizeof(*__inputs);
        }

        int average(){
            // Return average of input size
            int sz = size();
            __total = 0;
            for (int i = 0; i < sz; i++){
                __total += __inputs[i];
            };
            return __total / sz;
        };

        void add(int ipt){
            __inputs[__count] = ipt;
            __count++;
            if (__count > size()){
                __count=0;
                };
        };

};

// Configure Pins
const int voltage_sensor_pin = A1;
const int output_pin = 5;
int output_option = 0;

// Electronics values
// V,input / V,pin = R,input / R,pin
const long r_pin = 1e4;
const long r_besides_pin = 1e5;
const long r_total = r_pin + r_besides_pin;
const float r_ratio = r_total/r_pin;    //      R,input / R,pin

const float voltage_threshold = 14.1;
int input_threshold_value;

int input_analog_value;
int analog_write_counter = 0;


PinInValues piv(20);
int average;


int setup_voltage_bit_threshold(){
    // Working constant
    float constant = 1;

    // Convert threshold voltage to pin voltage
    constant *= voltage_threshold/r_ratio;

    // Convert pin voltage to 10bits
    // constant = map(constant, 0, 5, 0, 1023);
    constant /= 5;
    constant *= 1023;

    // Return int value for analog read threshold
    return int(constant);
};

void update_output_pin(){
    // Get analogRead value
    input_analog_value = analogRead(voltage_sensor_pin);
    switch(output_option){
        case 1:
            if (input_analog_value > input_threshold_value){
                analog_write_counter--;
            }
            else{
                analog_write_counter++;
            }
            analogWrite(output_pin, analog_write_counter);
            break;
        case 2:
            piv.add(input_analog_value);
            average = piv.average();
            if(average > input_threshold_value){
                digitalWrite(output_pin, LOW);
            }
            else{
                digitalWrite(output_pin, HIGH);
            }
            break;
        default:
            if(input_analog_value > input_threshold_value){
                digitalWrite(output_pin, LOW);
            }
            else {
                digitalWrite(output_pin, HIGH);
            }
            break;
    }
};

void setup(){
    Serial.begin(9600);
    pinMode(output_pin, OUTPUT);
    input_threshold_value = setup_voltage_bit_threshold();
};


void loop(){
    update_output_pin();
    Serial.print(input_analog_value);
    Serial.print(",");
    Serial.println(input_threshold_value);
};