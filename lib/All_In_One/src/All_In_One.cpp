/* 
    All_In_One.cpp - Library for controlling a motorcycle using an Arduino.
    Created by Darnell Baird, January 8th, 2021.
    Released into the public domain.
 */

#include "Arduino.h"

#include <TM1638plus.h>

#include "All_In_One.h"


TM1638plus tm(4,6,7);

void Electric_Load::turn_ON()
{
    current_state = !default_state;
    time_ON = millis();
};
void Electric_Load::turn_OFF()
{
    current_state = default_state;
    time_OFF = millis();
}
void Electric_Load::toggle()
{
    current_state = !current_state;
    time = millis();
    if (!is_ON())
    {
        time_OFF = time;
    }
    else
    {
        time_ON = time;
    }
    time_toggled = time;
}
bool Electric_Load::is_ON()
{
    return current_state == !default_state;
}
Electric_Load::Electric_Load(
    const int _id,
    const int _output_pin,
    const int _default_state,
    const int _control_type)
{
    // Assign attributes
    this->id = _id;
    this->output_pin = _output_pin;
    this->default_state = _default_state;
    this->control_type = _control_type;

    // Set pin mode
    pinMode(output_pin, OUTPUT);
}
void Electric_Load::update(int milliseconds)
{
    switch (control_type)
    {
    case states::control_type::blink:
        if (!is_ON()) // OFF
        {
            digitalWrite(output_pin, default_state);
        }
        else
        // Cycle ON and OFF.
        {
            if (milliseconds % time_blink_cycle < time_blink_cycle / 2)
            { // ON
                digitalWrite(output_pin, !default_state);
            }
            else
            { // OFF
                digitalWrite(output_pin, default_state);
            };
        }
        break;
    default:
        if (!is_ON())
        {
            digitalWrite(output_pin, default_state);
        } // OFF
        else
        {
            digitalWrite(output_pin, !default_state);
        } // ON
    }
};
void Electric_Load::update()
{
    update(millis());
};

All_In_One::All_In_One(const Electric_Load _loads[], const int _n_loads)
{
    // Assign attributes
    // this->loads[_n_loads];
    for (int i = 0; i < _n_loads; i++)
    {
        loads[i] = _loads[i];
    }
    n_loads = _n_loads;
};

void All_In_One::setup()
/* Call within 'setup' for Arduino. */
{
    // TM1638 Module
    // tm = TM1638plus(TM_STROBE, TM_CLOCK, TM_DIO, TM_HIGH_FREQ);
    tm.displayBegin();
    // tm.displayText("Baird");
    displayText("Baird");
};

void All_In_One::update()
/* Run update within 'update' of Arduino. */
{
    // Run update for each rider control
    current_millis = millis();
    for (int i = 0; i < n_loads; i++)
    {
        loads[i].update(current_millis);
    };
    // Update on TM1638 module
    tm_update();
};
void All_In_One::tm_update()
{
    // Update buttons status
    previous_buttons = current_buttons;
    current_buttons = tm.readButtons();
    // If any buttons pressed and previous buttons and current buttons match, it is held down
    is_held_down = current_buttons && (current_buttons == previous_buttons);
    just_pressed = current_buttons > previous_buttons;
    just_released = current_buttons < previous_buttons;
    // Debugging with some serial prints
    if (false){
        // Serial.println("Meow:");
        uint8_t checks = 0;
        // checks = current_buttons;
        checks = checks | (is_held_down<<0);
        checks = checks | (just_pressed<<1);
        checks = checks | (just_released<<2);
        // Serial.printf()
        Serial.println(checks, BIN);
    }

    Serial.println("We Reached");
    // Respond according to config
    switch(current_config)
    {
        case cluster::config::simple: config_simple_update(); break;
    }
}
void All_In_One::displayText(const char text[])
{
    tm.reset();
    if(to_displayText)
    {
        tm.displayText(text);
    }
}
void All_In_One::config_simple_update()
/* Carry-out actions based on buttons and combinations pressed. */
{
    switch(current_state)
    {
        case cluster::view::main: simple_main_view_update(); break;
    }
}
void All_In_One::simple_main_view_update()
{
    // get_load(states::load_id::left_indicator).turn_ON();
    // get_load(states::load_id::right_indicator).turn_ON();
    digitalWrite(get_load(states::load_id::left_indicator).output_pin, LOW);
    delay(3000);
    digitalWrite(get_load(states::load_id::left_indicator).output_pin, HIGH);
    // get_load(states::load_id::left_indicator).turn_OFF();
    // get_load(states::load_id::right_indicator).turn_OFF();
    delay(3000);
}
void All_In_One::simple_main_view_update1()
{
    if (just_pressed)
    {
        switch(current_buttons)
        {
            case 1<<0:  // Left indicator, left button
                // If other indicator is ON already, turn them OFF and turn this ON.
                if (get_load(states::load_id::right_indicator).is_ON())
                {
                    turn_ON(states::load_id::left_indicator);
                    displayText("Left ON.");
                }
                else
                // toggle left indicator
                {
                    toggle(states::load_id::left_indicator);
                    displayText("Left Tg.");
                }
                break;

            case 1<<7: // Right indicator, right button
                // If other indicator is ON already, turn them OFF and turn this ON.
                if (get_load(states::load_id::left_indicator).is_ON())
                {
                    turn_ON(states::load_id::right_indicator);
                    displayText("Rite ON.");
                }
                else
                // toggle right indicator
                {
                    toggle(states::load_id::right_indicator);
                    displayText("Rite Tg.");
                }
                break;

            case (1<<0) & (1<<7):   // Hazards (left and right indicators), left and right most buttons
                {   // https://stackoverflow.com/a/44336550/6556801
                    Electric_Load left = get_load(states::load_id::left_indicator);
                    Electric_Load right = get_load(states::load_id::right_indicator);
                    if (left.is_ON() && right.is_ON())
                    // Hazards is already ON. Turn it OFF.
                    {
                        left.turn_OFF();
                        right.turn_OFF();
                        displayText("Haz OFF.");
                    }
                    else
                    // An indicator might be ON already but turn ON hazards.
                    {
                        left.turn_OFF();
                        right.turn_OFF();
                        // Turn ON Hazards
                        left.turn_ON();
                        right.turn_ON();
                        displayText("Haz ON.");
                    }
                }

            case 1<<4:  // Headlight
                toggle(states::load_id::headlight);
                displayText("Head Tg.");
                break;
            
            case 1<<5:  // Lights
                toggle(states::load_id::lights);
                displayText("Lite Tg.");
                break;

            case 1<<6:  // Turn ON Horn
                // toggle(states::load_id::horn);
                turn_ON(states::load_id::horn);
                displayText("Horn ON.");
                break;

            case 1<<3:  // Turn ON ignition and attempt to start.
                {   // https://stackoverflow.com/a/44336550/6556801
                    Electric_Load ignition = get_load(states::load_id::ignition);
                    ignition.toggle();
                    // If ignition turn ON, turn ON starter
                    if(ignition.is_ON())
                    {
                        turn_ON(states::load_id::starter);
                    }
                    // display
                    // if(ignition.is_ON()){displayText("Ign ON.");}
                    // else{displayText("Ign OFF.");}
                }
        }
    }
    if(just_released)
    {
        // Turn OFF Horn
        if( !((1<<6) & current_buttons) & get_load(states::load_id::horn).is_ON())
        // Horn button not engaged.
        {
            turn_OFF(states::load_id::horn);
            displayText("Horn OFF.");
        }

        // Turn OFF starter
        if( !((1<<3) & current_buttons) )
        {
            turn_OFF(states::load_id::starter);
        }
    }
}

Electric_Load All_In_One::get_load(int id)
{
    /* Get Electric_Load object stored in All_In_One.
    If it doesn't exist, return a default Electric_Load
    */
    for (int i = 0; i < n_loads; i++)
    {
        if (id == loads[i].id)
        {
            return loads[i];
        }
    }
    return Electric_Load();
}
void All_In_One::turn_ON(int id)
{
    /* Turn an electric load ON. */
    get_load(id).turn_ON();
}
void All_In_One::turn_OFF(int id)
{
    /* Turn an electric load ON. */
    get_load(id).turn_OFF();
}
void All_In_One::toggle(int id)
{
    /* Turn an electric load ON. */
    get_load(id).toggle();
}
