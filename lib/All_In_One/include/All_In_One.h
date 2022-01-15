/* 
    All_In_One.h - Library for controlling a motorcycle using an Arduino.
    Created by Darnell Baird, January 13th, 2022.
    Released into the public domain.
 */

#ifndef All_In_One_h
#define All_In_One_h

#include "Arduino.h"

#include <TM1638plus.h>

#define TIME_BLINK_CYCLE_DEFAULT 1000

namespace states{
    enum load_type{button, blink};
    enum load_id{
        none,
        left_indicator,
        right_indicator,
        headlight,
        lights,
        brake,
        horn,
        starter,
        ignition
    };
    enum control_type{
        none,
        button,
        blink
    };
};

class Electric_Load
{
    public:
        int id = states::load_id::none;
        int output_pin;
        int default_state = LOW;
        int control_type = states::control_type::blink;
        
        void turn_ON();
        void turn_OFF();
        void toggle();
        bool is_ON();
        Electric_Load(){};
        Electric_Load(const int output_pin) : output_pin(output_pin){};
        Electric_Load(const int _id, const int _output_pin, const int _default_state, const int _control_type);
        
        void update(const int milliseconds);
        void update();
    private:
        int current_state = default_state;

        int time;
        int time_ON;
        int time_OFF;
        int time_toggled;
        int time_blink_cycle = TIME_BLINK_CYCLE_DEFAULT;
};

class All_In_One
{
public:
    int TM_CLOCK = 4;
    int TM_STROBE = 6;
    int TM_DIO = 7;
    int current_state = cluster::state::main;
    int current_config = cluster::config::simple;
    // Constructor
    All_In_One(const Electric_Load _loads[], const int _n_loads);
    // Accessor
    Electric_Load get_load(const int id);

    void setup();
    void update();

    void turn_OFF(const int load_id);
    void turn_ON(const int load_id);
    void toggle(const int load_id);

private:
    int current_millis;

    Electric_Load* loads;
    int n_loads;

    TM1638plus tm;
    uint8_t current_buttons;
    uint8_t previous_buttons;
    bool is_held_down;
    bool just_pressed;
    bool just_released;
    void tm_update();
    void config_simple_update();

};

#endif