/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Field_Coil_Regulator.h"

int Field_Coil_Regulator::__voltage_to_10bits(float voltage_limit)
{
  // Working constant
  float constant = 1;

  // Convert threshold voltage to pin voltage
  constant *= voltage_limit / __r_ratio;

  // Convert pin voltage to 10bits
  // constant = map(constant, 0, 5, 0, 1023);
  constant /= 5;
  constant *= 1023;

  // Return int value for analog read threshold
  return int(constant);
};

void Field_Coil_Regulator::set_input_upper_limit(float upperLimit)
{
  // Set the new voltage value
  voltage_upper_limit = constrain(upperLimit, voltage_lower_limit, MAX_VOLTAGE);
  // Now recalculate values
  __calc_input_limits();
};

void Field_Coil_Regulator::set_input_lower_limit(float lowerLimit)
{
  // Set the new voltage value
  voltage_lower_limit = constrain(lowerLimit, MIN_VOLTAGE, voltage_upper_limit);
  // Now recalculate values
  __calc_input_limits();
};

void Field_Coil_Regulator::__calc_input_limits()
{
  /* Calculate and set limits for each input_read threshold/limit */
  __input_lower_limit = __voltage_to_10bits(voltage_lower_limit);
  __input_upper_limit = __voltage_to_10bits(voltage_upper_limit);
};

void Field_Coil_Regulator::set_resistances(long sensingResistor, long otherResistor)
{
  /* Apply resistor values and then recalculate values depending on resistor values */
  // Set Resistance Values
  __r_between_pins = constrain(sensingResistor, MIN_RESISTANCE, MAX_RESISTANCE);
  __r_besides_pin = constrain(otherResistor, MIN_RESISTANCE, MAX_RESISTANCE);
  /* TODO: Give a warning?... if the resistance isn't adequate 
  for the voltages to protect the arduino pin
  */
  // Update dependent values
  __calc_resistances();
}

void Field_Coil_Regulator::__calc_resistances()
{
  // Sets final resistance values.
  __r_total = __r_between_pins + __r_besides_pin;
  __r_ratio = __r_total / __r_between_pins;
};

Field_Coil_Regulator::Field_Coil_Regulator(int upperInputAnalogPin, int lowerInputAnalogPin, int outputPWMPin)
{
  // Set pins modes and attribute
  pinMode(outputPWMPin, OUTPUT);
  pinMode(lowerInputAnalogPin, INPUT);
  pinMode(upperInputAnalogPin, INPUT);
  __OUTPUT_PIN = outputPWMPin;
  __VOLTAGE_SENSOR_PIN_UPPER = upperInputAnalogPin;
  __VOLTAGE_SENSOR_PIN_LOWER = lowerInputAnalogPin;

  // Setup base values
  setup();
}

void Field_Coil_Regulator::setup()
{
  /* Setup base values */
  __calc_resistances();
  __calc_input_limits();
};

void Field_Coil_Regulator::update(){
  /* Applies the appropriate response to the output pin 
  based on reading of input pins. */
  
  // Algorithm based on 'case 30'
  __input_analog_value_lower = analogRead(__VOLTAGE_SENSOR_PIN_LOWER);
  __input_analog_value_diff = __input_analog_value_upper - __input_analog_value_lower;
  if (__input_analog_value_diff < __input_lower_limit)
  {
      // Too low
      __analog_write_counter++;
  }
  else
  {
      if(__input_analog_value_diff > __input_upper_limit)
      {
          // Too high
          __analog_write_counter--;
      }
      // else: just right
  }

};