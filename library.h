#ifndef MOTOR_EMULATOR_LIBRARY_H
#define MOTOR_EMULATOR_LIBRARY_H

#include <csignal>
#include <windows.h>
class Motor_Emulator {

public:
    Motor_Emulator(int Volatge, int No_Load_Speed_RPM, double No_Load_Current_mA, double Stall_Current_A,
                       double Terminal_Resistance_ohm, double Terminal_Inductance_mH, double Torque_Constant_mNm_A,
                       int Speed_Constant_rpm_mNm, double Rotor_Inertia_gcm2);
    enum time_scale{seconds, milliseconds, miniutes};
    enum distance_scale{radians,};
    enum speed_scale{radians_second, rpm, rps};
    void RUN(double time, time_scale scale){};
    double GetCurrant(){};
    double GetDistance(distance_scale scale ){};
    double GetSpeed(speed_scale scale){};
    int SetPWM(int pwm){};
    void SetHallPattern(int A_pattern[],int B_pattern[], int no_positions){};
    void AttachHallInterrupt( void(*foo)()){
        Hall_Interrupt = foo;
    };
    void(*Hall_Interrupt)();
    void PORTD_Interrupt();
    void Start_Emulation(int Voltage,int PWM);
    void Pause_Emulation();
    void Stop_Emulation();
    void Place_Load_on_motor();
    void Change_Voltage(int Voltage);
    void Setup_Interrupts();


private:
    int A_Hall_pattern[];
    int B_Hall_pattern[];
    int pwm;
};

#endif