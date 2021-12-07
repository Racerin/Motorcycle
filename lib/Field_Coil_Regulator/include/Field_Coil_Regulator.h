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
    Field_Coil_Regulator(int upperInputAnalogPin, int lowerInputAnalogPin, int outputPWMPin);
    void setup();
    void update();

    // Voltage Thresholds and Limits
    const float MAX_VOLTAGE = 40;
    const float MIN_VOLTAGE = 0;
    float voltage_upper_limit = 12.8;
    float voltage_lower_limit = 14.1;

private:
    // Pins
    int __VOLTAGE_SENSOR_PIN_UPPER;
    int __VOLTAGE_SENSOR_PIN_LOWER;
    int __OUTPUT_PIN;

    // Electronic Hardware
    const long MAX_RESISTANCE = 1e7;
    const long MIN_RESISTANCE = 10;

    long __r_between_pins = 1e4;
    long __r_besides_pin = 1e5;
    long __r_total;
    float __r_ratio;

    int __input_threshold_value;
    int __input_upper_limit;
    int __input_lower_limit;

    int __input_analog_value_lower;
    int __input_analog_value_upper;
    int __input_analog_value_diff;

    int __analog_write_counter = 0;


    // METHODS
    int __voltage_to_10bits(float voltage_limit);

    void set_input_upper_limit(float upperLimit);
    void set_input_lower_limit(float lowerLimit);

    void set_resistances(long sensingResistor, long otherResistor);

    void __calc_resistances();
    void __calc_input_limits();
};

#endif