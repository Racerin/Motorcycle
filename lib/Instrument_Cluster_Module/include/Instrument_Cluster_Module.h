/* 
   Instrument_Cluster.h - Library for interfacing with the TM1638 module as an Instrument Cluster on a motorcycle.
   Created by Darnell Baird, January 11th, 2022.
   Released into the public domain.
 */

#ifndef Instrument_Cluster_Module_h
#define Instrument Cluster_Module_h

#include <Arduino.h>

class Instrument_Cluster_Module
{
public:
    // Instantiation
    Instrument_Cluster_Module(int clk, int talk, int dio_in, int dio_out);
    Instrument_Cluster_Module();

    // Methods
    void setup();
    void update();

  private:
    int clk;
};

#endif