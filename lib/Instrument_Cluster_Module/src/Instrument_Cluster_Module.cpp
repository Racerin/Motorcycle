/* 
    Instrument_Cluster.cpp - Library for interfacing with the TM1638 module for motorcycle
    Created by Darnell Baird, January 11th, 2021.
    Released into the public domain.
 */

#include <Arduino.h>
#include <TM1638.h>
#include <InvertedTM1638.h>

Instrument_Cluster_Module::Instrument_Cluster_Module(int clk, int talk, int dio_in, int dio_out)
{
    // TM1638 display = TM1638
    TM1638 display(3,2,4);
};
Instrument_Cluster_Module::Instrument_Cluster_Module();

void Instrument_Cluster_Module::setup(){};
void Instrument_Cluster_Module::update(){};