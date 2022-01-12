/* 
    Central_Control_Module.cpp - Library for controlling a motorcycle using an Arduino.
    Created by Darnell Baird, January 8th, 2021.
    Released into the public domain.
 */

#include "Arduino.h"

#include <ArduinoSTL.h>
#include <TM1638plus.h>

#include "Central_Control_Module.h"

namespace cluster{
    enum config{simple};
    enum state{main}; 
}

class Rider_Control_Factory
{
public:
    int input_pin;
    int output_pin;
    char type_flag;

    int state_OFF;
    int state;

    int time_blink_cycle = 1000;
    int time_blink_state;
    int time_last_pressed;
    int time_held_down;

    Rider_Control_Factory(int input_pin, int output_pin, int state_OFF, char type_flag)
    {
        // Set pins modes
        pinMode(input_pin, INPUT);
        pinMode(output_pin, OUTPUT);
    }

    Rider_Control_Factory() {}

    void activate_func()
    {
        digitalWrite(output_pin, !state_OFF);
    }
    void deactivate_func()
    {
        digitalWrite(output_pin, state_OFF);
    }
    void blink_activate_func(int curr_millis)
    {
        time_held_down = curr_millis - time_last_pressed;
        time_blink_state = (time_held_down / (time_blink_cycle / 2)) % 2;
        if (time_blink_state)
        {
            activate_func();
        }
        else
        {
            deactivate_func();
        };
    }
    void blink_activate_func()
    {
        blink_activate_func(millis());
    }

    void update(int current_milliseconds)
    /* 
        Assign methods to class based on flag_type.
        t : toggle (default)
        b : blink/indicator (toggle with cycling ON and OFF)
    */
    {
        /* Update pinout value based on runtime variables */
        switch (type_flag)
        {
        case 'b':
            // turn ON/OFF according to time cycle period
            if (state != state_OFF)
            {
                blink_activate_func(current_milliseconds);
            }
            else
            {
                deactivate_func();
            };
        default:
            // Simple toggle ON/OFF
            if (state != state_OFF)
            {
                activate_func();
            }
            else
            {
                deactivate_func();
            }
        };
    };

    void toggle()
    {
        /* Alternate the state. */
        // Set internal state
        state = !state;
    };

    void activate()
    {
        /* Set the state as active. */
        // Assign attributes
        time_last_pressed = millis();
        state = !state_OFF;
    };

    void deactivate()
    {
        /* Set the state as deactive. */
        state = state_OFF;
    };
};

class Instrument_Cluster_Module
{
public:
    // Attributes
    TM1638plus tm;
    cluster::state current_state = cluster::state::main;
    cluster::config current_config = cluster::config::simple;

    // Configuration
    void SimpleControls()
    {
        uint8_t buttons = tm.readButtons();
        switch (current_state)
        {
            case main:
                switch(buttons)
                {
                    // Left indicator, button 1
                    case 0b00000001: 

                }
                break;
        };
    };

    // Methods
    void setup()
    {
        TM1638plus tm(strobe, clk, dio, HIGH_FREQ);
    }
    void update();

    // Instantiation
    Instrument_Cluster_Module(int clk, int strobe, int dio)
    {
        // TM1638plus tm(strobe, clk, dio, HIGH_FREQ);
    };
    Instrument_Cluster_Module();

  private:
    bool HIGH_FREQ = false;
    
    int clk;
    int strobe;
    int dio;
};


Central_Control_Module::Central_Control_Module(
    electric_load ids[],
    int input_pins[],
    int output_pins[],
    int default_values[],
    char rider_control_type[],
    int n_pins)
{
    // Create arrays of size
    Rider_Control_Factory rider_controls[n_pins];

    // Create Rider_Control object and keep it within
    for (int i = 0; i < n_pins; i++)
    {
        Rider_Control_Factory rider_control = Rider_Control_Factory(
            input_pins[i],
            output_pins[i],
            default_values[i],
            rider_control_type[i]);
        rider_controls[i] = rider_control;

        // Update map
        rider_dict[ids[i]] = rider_control;
    }

    // Assign attributes
    __n_pins = n_pins;
    // __ids = new int[__n_pins];  // Creates a new array with the correct length
};

void Central_Control_Module::setup() //init
    {
        /* Run setup within 'setup' for Arduino. */
    };

void Central_Control_Module::update()
{
    /* Run update within 'update' of Arduino. */
    // Run update for each rider control
    current_millis = millis();
    for (int i = 0; i < __n_pins; i++)
    {
        rider_controls[i].update(current_millis);
    };
};
Rider_Control_Factory Central_Control_Module::get_rider_control_OLD(electric_load id)
{
    /* Get a Rider_Control_Factory object stored in rider_control 
    of Central_Control_Module.
    If it doesn't exist, return a default Rider_Control_Factory
    */
    for (int i = 0; i < __n_pins; i++)
    {
        //    if ( (int)id == rider_controls[i].id)
        if ((int)id == __ids[i])
        {
            return rider_controls[i];
        }
    }
    return Rider_Control_Factory();
}
Rider_Control_Factory Central_Control_Module::get_rider_control(electric_load id)
{
    /* Get a Rider_Control_Factory object stored in rider_control 
    of Central_Control_Module.
    */
    return rider_dict[id];
}
void Central_Control_Module::turn_ON(electric_load id)
{
    /* Turn an electric load ON. */
    get_rider_control(id).activate();
}
void Central_Control_Module::turn_OFF(electric_load id)
{
    /* Turn an electric load ON. */
    get_rider_control(id).deactivate();
}
void Central_Control_Module::toggle(electric_load id)
{
    /* Turn an electric load ON. */
    get_rider_control(id).toggle();
}