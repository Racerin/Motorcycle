/* 
    Central_Control_Module.h - Library for controlling a motorcycle using an Arduino.
    Created by Darnell Baird, January 8th, 2022.
    Released into the public domain.
 */

#ifndef Central_Control_Module_h
#define Central_Control_Module_h

#include "Arduino.h"
// #include <ArduinoSTL.h>
#include <map>

class Central_Control_Module
{
    public:
        // enum
        enum class electric_load{
            none=0,
            left_indicator, 
            right_indicator, 
            headlight, 
            horn, 
            lights, 
            ignition,
            brake,
            starter
            };

        // Instantiation
        Central_Control_Module(
            // int ids[],
            electric_load ids[],
            int input_pins[],
            int output_pins[],
            int default_values[],
            char rider_control_types[],
            int n_pins
            );
        
        // Methods
        void setup();
        void update();
        void turn_OFF(electric_load load_id);
        void turn_ON(electric_load load_id);
        void toggle(electric_load load_id);

    private:
        // Attributes
        int __n_pins;
        int current_millis;
        int* __ids; // Will be populated later
        std::map<electric_load, Rider_Control_Factory> rider_dict;
        // std::vector<int> __ids;

        Rider_Control_Factory rider_controls[];

        // Methods
        Rider_Control_Factory get_rider_control_OLD(electric_load id);
        Rider_Control_Factory get_rider_control(electric_load id);

};

#endif