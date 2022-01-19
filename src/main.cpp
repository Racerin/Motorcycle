#include <Arduino.h>

#include <All_In_One.h>

using namespace states;

// Electric Loads
Electric_Load loads[] = {
    Electric_Load(load_id::left_indicator, A0, HIGH, control_type::blink),
    Electric_Load(load_id::right_indicator, A1, HIGH, control_type::blink),
    Electric_Load(load_id::headlight, A2, HIGH, control_type::button),
    Electric_Load(load_id::lights, A3, HIGH, control_type::button),
    Electric_Load(load_id::brake, A4, LOW, control_type::button),
    Electric_Load(load_id::horn, A5, HIGH, control_type::button),
    Electric_Load(load_id::starter, A6, HIGH, control_type::button),
    Electric_Load(load_id::ignition, A7, HIGH, control_type::button),
};

All_In_One ccm( loads, sizeof(loads)/sizeof(loads[0]) );


void setup()
{
    // Print setup
    Serial.begin(19200);
    Serial.println("This is the start.");

    // put your setup code here, to run once:
    ccm.TM_STROBE = 4;
    ccm.TM_CLOCK = 6;
    ccm.TM_DIO = 7;
    ccm.setup();
    Serial.println("End of setup.");
}

void loop()
{
    // Serial.println("Hello.");
    // put your main code here, to run repeatedly:
    ccm.update();

}