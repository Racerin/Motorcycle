/* 
   Instrument_Cluster.h - Library for interfacing with the TM1638 module as an Instrument Cluster on a motorcycle.
   Created by Darnell Baird, January 11th, 2022.
   Released into the public domain.
 */

#ifndef Instrument_Cluster_Module_h
#define Instrument Cluster_Module_h

#include <Arduino.h>

#include <TM1638plus.h>


namespace cluster{
    enum config{simple};
    enum state{main}; 
}

class Instrument_Cluster_Module
{
public:
    // Attributes
    TM1638plus tm;
    cluster::state current_state = cluster::state::main;
    cluster::config current_config = cluster::config::simple;

    // Methods
    void setup();
    void update();

    // Instantiation
    Instrument_Cluster_Module(int clk, int strobe, int dio);
    Instrument_Cluster_Module();

    // Configuration
    void SimpleControls();

  private:
    int clk;
    int strobe;
    int dio;
};

#endif