/* 
    Central_Control_Module.h - Library for controlling a motorcycle using an Arduino.
    Created by Darnell Baird, January 8th, 2022.
    Released into the public domain.
 */

#ifndef Central_Control_Module_h
#define Central_Control_Module_h

#include "Arduino.h"

class Central_Control_Module
{
    public:
        Central_Control_Module(
            int input_pins[],
            int output_pins[],
            int default_values[],
            char rider_control_types[],
            int n_pins
            );
        void setup();
        void update();
        int turn_on_indicator_left();
        int turn_on_indicator_right();
        enum class electric_load{
            left_indicator, 
            right_indicator, 
            headlight, 
            horn, 
            lights, 
            ignition,
            brake,
            starter
            };
    private:
        int __n_pins;

        Rider_Control_Factory rider_controls[];

};

#endif