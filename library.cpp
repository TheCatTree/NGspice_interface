#include "library.h"



Motor_Emulator::Motor_Emulator(int Volatge, int No_Load_Speed_RPM, double No_Load_Current_mA, double Stall_Current_A,
                               double Terminal_Resistance_ohm, double Terminal_Inductance_mH,
                               double Torque_Consatant_mNm_A, int Speed_Constant_rpm_mNm, double Rotor_Inertia_gcm2) {

}
void Motor_Emulator::PORTD_Interrupt() {
    raise(SIGINT);
}