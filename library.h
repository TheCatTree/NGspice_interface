#ifndef MOTOR_EMULATOR_LIBRARY_H
#define MOTOR_EMULATOR_LIBRARY_H

#include <csignal>
#include "lib/Spice64_dll/include/ngspice/sharedspice.h"
#include <vector>
#include "tests/MockArduino.h"
#include <string>
#include <map>

struct Time_line{
    double Start_Time_S;
    double Time_line_length_S;
    long Time_line_length_points;
    std::map<std::string,vector_info> v_info;

};


class Motor_Emulator {

public:
    // Nominal voltage V, No load speed rpm, No load current mA, stall current A, Terminal Resistance ohm, Terminal inductance mH, Torque constant mNm/A, Speed constant rpm/Vrotor inertia
    Motor_Emulator(int Voltage, int No_Load_Speed_RPM, double No_Load_Current_mA, double Stall_Current_A,
                       double Terminal_Resistance_ohm, double Terminal_Inductance_mH, double Torque_Constant_mNm_A,
                       int Speed_Constant_rpm_mNm, double Rotor_Inertia_gcm2)
            : Voltage(Voltage), No_Load_Speed_RPM(No_Load_Speed_RPM), No_Load_Current_mA(No_Load_Current_mA),Stall_Current_A(Stall_Current_A),
    Terminal_Resistance_ohm(Terminal_Resistance_ohm), Terminal_Inductance_mH(Terminal_Inductance_mH), Torque_Constant_mNm_A(Torque_Constant_mNm_A),
    Speed_Constant_rpm_mNm(Speed_Constant_rpm_mNm),Rotor_Inertia_gcm2(Rotor_Inertia_gcm2){}
    enum time_scale{seconds, milliseconds, miniutes};
    enum distance_scale{radians,};
    enum speed_scale{radians_second, rpm, rps};
    void RUN(double time, time_scale scale){};
    double GetCurrant(){};
    double GetDistance(distance_scale scale ){};
    double GetSpeed(speed_scale scale){};
    int SetPWM(int pwm){};
    void SetHallPattern(std::vector<int> &A_pattern,std::vector<int> &B_pattern);
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
    std::vector<int> A_Hall_pattern;
    std::vector<int> B_Hall_pattern;
    int pwm;
    int Voltage; int No_Load_Speed_RPM; double No_Load_Current_mA; double Stall_Current_A;
    double Terminal_Resistance_ohm; double Terminal_Inductance_mH; double Torque_Constant_mNm_A;
    int Speed_Constant_rpm_mNm; double Rotor_Inertia_gcm2;
};





#endif