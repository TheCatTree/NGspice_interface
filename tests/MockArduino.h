#ifndef DUEL_MOTOR_DRIVER_ARDUINO_LIBS_MOCKARDUINO_H
#define DUEL_MOTOR_DRIVER_ARDUINO_LIBS_MOCKARDUINO_H

#include <chrono>
unsigned long millis(){
    std::chrono::high_resolution_clock::duration dtn = std::chrono::high_resolution_clock::now().time_since_epoch();
    return (unsigned long)std::chrono::duration_cast<std::chrono::milliseconds>(dtn);
}

unsigned long micros(){
    std::chrono::high_resolution_clock::duration dtn = std::chrono::high_resolution_clock::now().time_since_epoch();
    return (unsigned long)std::chrono::duration_cast<std::chrono::microseconds>(dtn);
}
void cli(){
    //Signal interrupt emulation off
};
void sei(){
    //Signal interrupt emulation on
};

template <typename T>
static inline T min(T a, T b) {
    return (a<b) ? a : b;
}

template <typename T>
static inline T max(T a, T b) {
    return (a>b) ? a : b;
}

template <typename T>
static inline T max(T x){
    return (x>=0) ? x : -x;
}

#endif //DUEL_MOTOR_DRIVER_ARDUINO_LIBS_MOCKARDUINO_H