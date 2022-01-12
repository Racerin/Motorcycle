/* 
    Instrument_Cluster.cpp - Library for interfacing with the TM1638 module for motorcycle
    Created by Darnell Baird, January 11th, 2021.
    Released into the public domain.
 */

#include <Arduino.h>

#include <TM1638plus.h>

bool HIGH_FREQ = false;

Instrument_Cluster_Module::Instrument_Cluster_Module(int clk, int strobe, int dio)
{
    TM1638plus tm(strobe, clk, dio, HIGH_FREQ);
};
// Instrument_Cluster_Module::Instrument_Cluster_Module();
void Instrument_Cluster_Module::SimpleControls()
/* Returns a simple preset options of rider controls. 
 */
{
};

void Instrument_Cluster_Module::setup(){
    TM1638plus tm(strobe, clk, dio, HIGH_FREQ);
};
void Instrument_Cluster_Module::update()
/* Select Preset depending on the configuration selected and
the current state of the cluster instrument.
*/
{
};