//
// Created by TCT on 6/07/2018.
//

#ifndef MOTOR_EMULATOR_SYSTEM_TIMER_H
#define MOTOR_EMULATOR_SYSTEM_TIMER_H
#include <atomic>
#include <chrono>
#include <iostream>


class system_timer {
public:
    int start();
    int stop();
    int reset();
    unsigned long get_time();
    bool get_clock_state();
private:
    std::atomic<bool> onORoff = {false};
    std::chrono::microseconds time_buffer = std::chrono::microseconds::zero();
    std::chrono::high_resolution_clock::time_point start_time =  std::chrono::high_resolution_clock::now();

};
#endif //MOTOR_EMULATOR_SYSTEM_TIMER_H
