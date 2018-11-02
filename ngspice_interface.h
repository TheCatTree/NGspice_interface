#ifndef NGSPICE_INTERFACE_H
#define NGSPICE_INTERFACE_H
#include <iostream>
#include "lib/Spice64_dll/include/ngspice/sharedspice.h"
#include <string>
#include <sstream>

namespace NGSIF{

    extern bool silence;
    extern bool logging_on;
    extern std::ostringstream outputstream;

    void Ng_spice_set_up();


}



#endif