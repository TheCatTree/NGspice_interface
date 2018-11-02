
#include "System_Timer.h"


int system_timer::stop() {
    if(onORoff == true){
        onORoff = false;
        std::chrono::microseconds x = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time);
        time_buffer =  x + time_buffer;
    }
    return 0;
}

int system_timer::start() {
    if(onORoff == false){
        onORoff = true;
        start_time = std::chrono::high_resolution_clock::now();
    }
    return 0;
}

int system_timer::reset() {
    start_time = std::chrono::high_resolution_clock::now();
    time_buffer = std::chrono::microseconds::zero();
    return 0;
}

unsigned long system_timer::get_time() {
    auto x =  time_buffer;
    if(onORoff == true){
        x = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start_time) + x;
    }
    return (unsigned long)x.count();
}

bool system_timer::get_clock_state() {
    return onORoff;
}
