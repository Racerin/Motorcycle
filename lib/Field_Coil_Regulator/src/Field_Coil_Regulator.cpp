/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/


#include "Arduino.h"
#include "Field_Coil_Regulator.h"


Field_Coil_Regulator::Field_Coil_Regulator(int inputAnalogPin, int outputPWMPin)
{
    pinMode(outputPWMPin, OUTPUT);
    OUTPUT_PIN = outputPWMPin;

    // Setup base values
    calc_resistances();
    calc_input_limits();
}

void Field_Coil_Regulator::setup()
{
  
};
void Field_Coil_Regulator::update(){};