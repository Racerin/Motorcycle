



#include <Instrument_Cluster_Module.h>

#define CLOCK 2
#define STROBE 2
#define DIO 2


Instrument_Cluster_Module icm(CLOCK, STROBE, DIO);

void setup(){
    icm.setup();
};

void loop(){
    icm.update();
    delay(1000);
};