/*
  Field_Coil_Regulator.h - Library for flashing Field_Coil_Regulator code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/


#ifndef Field_Coil_Regulator_h
#define Field_Coil_Regulator_h


#include "Arduino.h"


class Field_Coil_Regulator
{
    public:
        Field_Coil_Regulator(int inputAnalogPin, int outputPWMPin);
        void setup();
        void update();
    private:
        // Pins
        int VOLTAGE_SENSOR_PIN;
        int OUTPUT_PIN;

        // Electronic Hardware
        const long R_PIN = 1e4;
        const long R_BESIDES_PIN = 1e5;
        long R_TOTAL;
        float R_RATIO;

        // Voltage Thresholds and Limits
        const float VOLTAGE_THRESHOLD = 14.1;
        const float voltage_upper_limit = 12.8;
        const float voltage_lower_limit = 14.1;

        int input_threshold_value;
        int input_upper_limit;
        int input_lower_limit;


        // METHODS
        
        int __voltage_to_10bits(float voltage_limit){
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

        void calc_resistances(){
            // Sets final resistance values.
            R_TOTAL = R_PIN + R_BESIDES_PIN;
            R_RATIO = R_TOTAL/R_PIN;
        }

        void calc_input_limits(){
            // Calculate and set limits for each input_read threshold/limit
            input_threshold_value = __voltage_to_10bits(VOLTAGE_THRESHOLD);
            input_lower_limit = __voltage_to_10bits(voltage_lower_limit);
            input_upper_limit = __voltage_to_10bits(voltage_upper_limit);
        }
};


#endif