#ifndef DUEL_MOTOR_DRIVER_ARDUINO_LIBS_MOCKARDUINO_H
#define DUEL_MOTOR_DRIVER_ARDUINO_LIBS_MOCKARDUINO_H
#include <chrono>
#undef min
#undef max
void setMockarduinoclockstartpoint(std::chrono::high_resolution_clock::time_point point);
unsigned long millis();

unsigned long micros();
void cli();
void sei();

template <typename T>
T min(T a, T b);

template <typename T>
T max(T a, T b);

template <typename T>
T max(T x);

#endif //DUEL_MOTOR_DRIVER_ARDUINO_LIBS_MOCKARDUINO_H