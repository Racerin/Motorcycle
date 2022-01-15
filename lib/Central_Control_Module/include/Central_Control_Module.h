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

// enum
enum load_id
{
    none,
    left_indicator,
    right_indicator,
    headlight,
    horn,
    lights,
    ignition,
    brake,
    starter
};

enum load_type{
    button,
    cycle
};

class Central_Control_Module
{
public:
    // Instantiation
    Central_Control_Module(
        int ids[],
        int output_pins[],
        int default_values[],
        int rider_control_types[],
        int n_pins);

    // Methods
    void setup();
    void update();
    void turn_OFF(int load_id);
    void turn_ON(int load_id);
    void toggle(int load_id);

private:
    // Attributes
    int n_pins;
    int current_millis;
    int ids;

    Instrument_Cluster_Module icm;
    Electric_Loads elecLoads;

    Electric_Load rider_controls[];

    // Methods
    Electric_Load get_rider_control(load_id id);
};

#endif