/* 
    Central_Control_Module.cpp - Library for controlling a motorcycle using an Arduino.
    Created by Darnell Baird, January 8th, 2021.
    Released into the public domain.
 */

#include "Arduino.h"
// #include <Bounce2.h>
// #include "InputDebounce.h"
#include "Central_Control_Module.h"

class Button
{
    /* Returns the states of a pin as needed. */
public:
    int pin;
    int time_now;
    int time_press_threshold = 100;
    int time_pressed_down;
    int is_pressing;
    int normal_value;
    int dt_press_down;

    Button(int pin, int normal_value, int time_press_threshold)
    {
        setup();
    };

    void setup()
    {
        // Set pin mode
        pinMode(pin, INPUT);
    };

    void update()
    {
        /* Update some key attributes */
        time_now = millis();
        dt_press_down = time_now - time_pressed_down;
    };

    int pressed()
    {
        /* Checks to see if button was pressed. */
        return digitalRead(pin) != normal_value;
    };

    int ON()
    {
        /* Check to see if button pressed based on hold down (debouncing) */
        update();
        if (pressed())
        {
            if (dt_press_down > time_press_threshold)
            {
                // button was pressed sufficiently
                return 1;
            }
            else
            {
                // Check if it is 1st press
                if (!is_pressing)
                {
                    // Time of 1st press
                    time_pressed_down = time_now;
                }
                is_pressing = 1;
            }
        }
        else
        {
            // updating values
            is_pressing = 0;
            // Just return false
            return 0;
        };
    };
};

class Rider_Control_Factory
{
public:
    int input_pin;
    int output_pin;
    char type_flag;

    int state;
    int state_OFF;

    int time_blink_cycle = 1000;
    int time_blink_state;
    int time_last_pressed;
    int time_held_down;

    Rider_Control_Factory(int input_pin, int output_pin, int state_OFF, char type_flag)
    {
        // Set pins modes
        pinMode(input_pin, INPUT);
        pinMode(output_pin, OUTPUT);
    };

    Rider_Control_Factory(){};

    void activate_func()
    {
        digitalWrite(output_pin, !state_OFF);
    }
    void deactivate_func()
    {
        digitalWrite(output_pin, state_OFF);
    }
    void blink_activate_func()
    {
        time_held_down = millis() - time_last_pressed;
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

    void update()
    /* 
        Assign methods to class based on flag_type.
        t : toggle (default)
        m : momentary[ switch]
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
                blink_activate_func();
            }
            else
            {
                deactivate_func();
            };
        default:
            // Do nothing
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

Central_Control_Module::Central_Control_Module(
    int input_pins[],
    int output_pins[],
    int default_values[],
    char rider_control_type[],
    int n_pins)
{
    // Create arrays of size
    Rider_Control_Factory rider_controls[n_pins];

    // Create Rider_Control object and keep it within
    for (int n = 0; n < n_pins; n++)
    {
        Rider_Control_Factory rider_control = Rider_Control_Factory(
            input_pins[n],
            output_pins[n],
            default_values[n],
            rider_control_type[n]);
        rider_controls[n] = rider_control;
    }

    // Assign attributes
    __n_pins = n_pins;
};

void Central_Control_Module::setup() //init
    {
        /* Run setup within 'setup' for Arduino. */
    };

void Central_Control_Module::update()
{
    /* Run update within 'update' of Arduino. */
    // Run update for each rider control
    for (int i = 0; i < __n_pins; i++)
    {
        rider_controls[i].update();
    };
};