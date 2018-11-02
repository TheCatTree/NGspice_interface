//
// Created by TCT on 5/06/2018.
//

#include <chrono>
#include <iostream>
#include "MockArduino.h"
std::chrono::high_resolution_clock::time_point MockArduino_global_clock;

void setMockarduinoclockstartpoint(std::chrono::high_resolution_clock::time_point point){
    MockArduino_global_clock = point;
    return;
};
unsigned long millis(){
    std::chrono::high_resolution_clock::duration dtn = (std::chrono::high_resolution_clock::now() - MockArduino_global_clock);
    std::chrono::milliseconds ms;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dtn);
    return (unsigned long)ms.count();

}

unsigned long micros(){
    std::chrono::high_resolution_clock::duration dtn =  (std::chrono::high_resolution_clock::now() - MockArduino_global_clock);
    std::chrono::microseconds us;
    us = std::chrono::duration_cast<std::chrono::microseconds>(dtn);
    return (unsigned long) us.count();
}
void cli(){
    //Signal interrupt emulation off
};
void sei(){
    //Signal interrupt emulation on
};

template <typename T>
T min(T a, T b) {
    return (a<b) ? a : b;
}

template <typename T>
T max(T a, T b) {
    return (a>b) ? a : b;
}

template <typename T>
T max(T x){
    return (x>=0) ? x : -x;
}

