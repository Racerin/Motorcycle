/* Code for arduino to regulate output voltage. 
Different approach to 'main'.
Get threshold as a 10bit vlue from the start and compare analogRead to 10bit threshold value.
*/

#include <Arduino.h>

int output_option = 30;

class PinInValues{
    private:
        int __inputs [10];
        int __count = 0;
        int __total = 0;
        int __max = 0;

    public:
        int avg;
        int maxm;

        PinInValues(int size){
            int __inputs[size] = {0};
        }

        int size(){
            return sizeof(__inputs)/sizeof(*__inputs);
        }

        int get_average(){
            // Return average of input size
            int sz = size();
            __total = 0;
            for (int i = 0; i < sz; i++){
                __total += __inputs[i];
            };
            return __total / sz;
        };

        int get_maximum(){
            int sz = size();
            int check;
            for(int i; i < sz; i++){
                check = __inputs[i];
                if(__max < check){
                    __max = check;
                }
            }
            return __max;
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
const int VOLTAGE_SENSOR_PIN_UPPER = A1;
const int VOLTAGE_SENSOR_PIN_LOWER = A2;

const int POTENTIOMETER_PIN = A5;
const int OUTPUT_PIN = 3;
char print_option;

// Electronics values
// V,input / V,pin = R,input / R,pin
const long R_PIN = 1e4;
const long R_BESIDES_PIN = 1e5;
const long R_TOTAL = R_PIN + R_BESIDES_PIN;
const float R_RATIO = R_TOTAL/R_PIN;    //      R,input / R,pin

// Thresholds and limits
const float VOLTAGE_THRESHOLD = 14.1;
int input_threshold_value;
int input_upper_limit;
int input_lower_limit;

const float VOLTAGE_UPPER_THRESHOLD = 14.1;
const float VOLTAGE_LOWER_THRESHOLD = 12.8;

// Input read values
int input_analog_value_upper;
int input_analog_value_lower;
int input_analog_value_diff;
int input_potentiometer_value;
int analog_write_counter = 0;

// Manual Adjust values
int pot_mapped;

PinInValues piv(20);


int setup_voltage_bit_threshold(float voltage_limit){
    // Working constant
    float constant = 1;

    // Convert threshold voltage to pin voltage
    constant *= voltage_limit/R_RATIO;

    // Convert pin voltage to 10bits
    // constant = map(constant, 0, 5, 0, 1023);
    constant /= 5;
    constant *= 1023;

    // Return int value for analog read threshold
    return int(constant);
};

void update_output_pin(){
    // Get analogRead value
    input_analog_value_upper = analogRead(VOLTAGE_SENSOR_PIN_UPPER);
    switch(output_option){
        case 1:
            // Joggle based on input value being below or above threshold
            if (input_analog_value_upper > input_threshold_value){
                analog_write_counter--;
            }
            else{
                analog_write_counter++;
            }
            analog_write_counter = constrain(analog_write_counter, 0, 255);
            analogWrite(OUTPUT_PIN, analog_write_counter);
            break;
        case 2:
            // Buffer based on average input value crossing threshold
            piv.add(input_analog_value_upper);
            piv.avg = piv.get_average();
            if(piv.avg > input_threshold_value){
                digitalWrite(OUTPUT_PIN, LOW);
            }
            else{
                digitalWrite(OUTPUT_PIN, HIGH);
            }
            break;
        case 3:
            // Buffer based on maximum input value crossing threshold
            piv.add(input_analog_value_upper);
            piv.maxm = piv.get_maximum();
            if(piv.maxm > input_threshold_value){
                digitalWrite(OUTPUT_PIN, LOW);
            }
            else{
                digitalWrite(OUTPUT_PIN, HIGH);
            }
            break;
        case 10:
            // Stay between lower and upper limits
            if (input_analog_value_upper < input_lower_limit)
            {
                analog_write_counter++;
                Serial.println("Too low");
            }
            else
            {
                if(input_analog_value_upper > input_upper_limit)
                {
                    analog_write_counter--;
                    Serial.println("Too high");
                }
                else{
                    Serial.println("Too just right.");
                }
            };
            analog_write_counter = constrain(analog_write_counter, 0, 255);
            analogWrite(OUTPUT_PIN, analog_write_counter);
            break;
        case 11:
            // Similiar to case 10 but actuation reverse because IGBT is now pull-up resistor
            // Stay between lower and upper limits
            if (input_analog_value_upper < input_lower_limit)
            {
                analog_write_counter--;
                Serial.println("Too low");
            }
            else
            {
                if(input_analog_value_upper > input_upper_limit)
                {
                    analog_write_counter++;
                    Serial.println("Too high");
                }
                else{
                    Serial.println("Too just right.");
                }
            };
            analog_write_counter = constrain(analog_write_counter, 0, 255);
            analogWrite(OUTPUT_PIN, analog_write_counter);
            break;
        case 30:
            /* Read the lower voltage analog pin, 
            find the difference between analog reads
            and use the value to determine output voltage.
            Similiar to case 10
              */
            input_analog_value_lower = analogRead(VOLTAGE_SENSOR_PIN_LOWER);
            input_analog_value_diff = input_analog_value_upper - input_analog_value_lower;
            if (input_analog_value_diff < input_lower_limit)
            {
                // Too low
                analog_write_counter++;
            }
            else
            {
                if(input_analog_value_diff > input_upper_limit)
                {
                    // Too high
                    analog_write_counter--;
                }
                // else: just right
            }
            break;
        case 20:
            // Manually adjust output pin's pwm according to value on potentiometer
            input_potentiometer_value = analogRead(POTENTIOMETER_PIN);
            // pot_mapped = map(input_potentiometer_value, 0, 1023, 0, 255);
            pot_mapped = input_potentiometer_value * 255 / 1023;
            // pot_mapped = input_potentiometer_value * 1023 / 255;
            analogWrite(OUTPUT_PIN, input_potentiometer_value);
            break;
        default:
            // Adjusts input_analog_value_upper according to above or below threshold
            if(input_analog_value_upper > input_threshold_value){
                digitalWrite(OUTPUT_PIN, LOW);
            }
            else {
                digitalWrite(OUTPUT_PIN, HIGH);
            }
            break;
    }
};

void adjust_timer(){
    // Adjust (increase) the timer frequency for pin 5 (and 6)
    // TCCR0B;  // Timer
    // CS00;   // Prescalar
    TCCR0B = TCCR0B & B11111000 | B00000001;    // PWM frequency of 31372.55 Hz: https://www.electronicwings.com/users/sanketmallawat91/projects/215/frequency-changing-of-pwm-pins-of-arduino-uno#:~:text=PWM%20is%20used%20by%20using,a%20simple%20line%20of%20code.
    // By changing Timer0, functions; millis, delay: are affect
    // For more info: https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
};

void setup(){
    // Setup serial connection
    Serial.begin(9600);

    // Setup pins
    pinMode(OUTPUT_PIN, OUTPUT);
    input_threshold_value = setup_voltage_bit_threshold(VOLTAGE_THRESHOLD);

    // Setup timer
    // adjust_timer();

    // case 10 threshold
    input_lower_limit = setup_voltage_bit_threshold(12.8);
    input_upper_limit = setup_voltage_bit_threshold(14.1);

    // Setup print option
    if (output_option < 10){print_option='a';}
    if (output_option == 20){print_option='b';}
};


void loop(){
    update_output_pin();
    // Print output
    switch(print_option)
    {
        case 'a':
            Serial.print(input_analog_value_upper);
            Serial.print(",");
            Serial.print(input_threshold_value);
            Serial.print("\n");
            break;
        case 'c':
            Serial.print(input_analog_value_upper);
            Serial.print(",");
            Serial.print(input_lower_limit);
            Serial.print(",");
            Serial.print(input_upper_limit);
            Serial.print("\n");
            break;
    }
};