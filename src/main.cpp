#include <Arduino.h>

#include <All_In_One.h>

using namespace states;

// Electric Loads
Electric_Load loads[] = {
    Electric_Load(load_id::left_indicator, A5, HIGH, control_type::button),
    Electric_Load(load_id::right_indicator, A6, HIGH, control_type::button),
    Electric_Load(load_id::headlight, A7, HIGH, control_type::button),
    Electric_Load(load_id::lights, 8, HIGH, control_type::button),
    Electric_Load(load_id::brake, 9, HIGH, control_type::button),
    Electric_Load(load_id::horn, 10, HIGH, control_type::button),
    Electric_Load(load_id::starter, 11, HIGH, control_type::button),
    Electric_Load(load_id::ignition, 12, HIGH, control_type::button),
};

All_In_One ccm( loads, sizeof(loads)/sizeof(loads[0]) );

uint8_t builtin_led_state = 0;

void setup()
{
    // Print setup
    Serial.begin(9600);

    // put your setup code here, to run once:
    ccm.TM_STROBE = 4;
    ccm.TM_CLOCK = 6;
    ccm.TM_DIO = 7;
    ccm.setup();
}

void loop()
{
    // put your main code here, to run repeatedly:
    ccm.update();

    // Update led
    builtin_led_state != builtin_led_state;
    digitalWrite(LED_BUILTIN, builtin_led_state);

}