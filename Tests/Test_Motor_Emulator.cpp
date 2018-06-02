//
// Created by TCT on 2/05/2018.
//

#include "catch.hpp"
#include "../library.h"
#include <cmath>
#include "MockArduino.h"
#define byte unsigned char
void Move_interrupt();
unsigned char PIND; // Must create the globals for the motor
byte M1_Mask = 0b00100100; // PD2= M1A PD5=M1B
byte M2_Mask = 0b10010000;// PD4=M2A PD7=M2B binary order 76543210
byte forward_table[10]  = {1,9,0,0,0,0,0,0,0,8};//commutation pattern for a an encoder moving foward
byte reverse_table[10] = {8,0,0,0,0,0,0,0,9,1};//commutation pattern for a an encoder moving backward

unsigned char Last_M1_value;

int M1_Position;

unsigned long M1_Time_Movement;
#define MOTORSPEEDARRAYSIZE 8
int Motor1Speed[MOTORSPEEDARRAYSIZE];

int M1Speedindex;

unsigned long M1_Time_Last_Movement;

int M1_New_Speed_Flag;

char M1_Leader;

unsigned char Last_M2_value;

char M2_Leader;

char M1_DOM;

int M1_Wanted_Position_as_int;

bool indexing_on;

int M2_Position;

unsigned long M2_Time_Movement;

int Motor2Speed[MOTORSPEEDARRAYSIZE];

int M2Speedindex;

unsigned long M2_Time_Last_Movement;

int M2_New_Speed_Flag;

int M2_Wanted_Position_as_int;

char M2_DOM;

SCENARIO("A Single Emulated motor Not attached to anything, with no interrupts"){
    GIVEN("Motor Set as a DCX 10 S Precious Metal Brush 12v"){
        Motor_Emulator ME(12, 12500, 10.5,0.16,74.9,0.868,8.53,1120,0.071);
        // Nominal voltage V, No load speed rpm, No load current mA, stall current A, Terminal Resistance ohm, Terminal inductance mH, Torque constant mNm/A, Speed constant rpm/V
        //Results of spice model PWM 10% incriments
        float Spice_PWM_Current[10] = {13.47,22.46,28.8,30.4,30.3,29,26.32,22.3,17.05,11.11};//MA
        float   Spice_PWM_Speed[10] = {144.13,288.30,432.4,567.4,720.5,838.6,956.6,1074.5,1192.3,1305.7}; //RPM //radians

        WHEN("Run with no load at 12v for 10 seconds"){
            ME.Start_Emulation(12,255);
            Sleep(10000);
            ME.Pause_Emulation();
            THEN("Motor current should = no load current (mA), and speed = no load speed"){
                REQUIRE(ME.GetCurrant() == 10.5);
                REQUIRE(ME.GetSpeed(Motor_Emulator::radians_second) == 12500);
                REQUIRE(ME.GetDistance(Motor_Emulator::radians) == (12500.0/60.0)*10*2*M_PI );//(RPS * Seconds * 2PI)
                ME.Stop_Emulation;
            }
        }

        WHEN("Run at 10% pwm increments"){
            for(int i = 0; i<10; i++){
                ME.Start_Emulation(12,i*10*(2.5));
                Sleep(10000);
                ME.Pause_Emulation;
                THEN("Settings should match Spice values"){
                    REQUIRE(ME.GetCurrant() == Spice_PWM_Current[i]);
                    REQUIRE(ME.GetSpeed(Motor_Emulator::radians_second) == Spice_PWM_Speed[i] );
                    REQUIRE(ME.GetDistance(Motor_Emulator::radians) == ((float)Spice_PWM_Speed[i]/60.0)*10*2*M_PI );//(RPS * Seconds * 2PI)
                    ME.Stop_Emulation;
                }
            }
        }

        WHEN("Run with full load at 12v for 10 seconds"){
            ME.Start_Emulation(12,255);
            ME.Place_Load_on_motor();
            Sleep(10000);
            ME.Pause_Emulation;
            THEN("Motor current should = no load current (mA), and speed = no load speed"){
                REQUIRE(ME.GetCurrant() == 160 );
                REQUIRE(ME.GetSpeed(Motor_Emulator::radians_second) == 0 );
                REQUIRE(ME.GetDistance(Motor_Emulator::radians) == (0/60.0)*10*2*M_PI );//(RPS * Seconds * 2PI)
                ME.Stop_Emulation;
            }
        }
    }
}

SCENARIO("Emulated Motor with interrupts threw the signal system"){
    GIVEN("Motor Set as a DCX 10 S Precious Metal Brush 12v"){
        Motor_Emulator ME(12, 12500, 10.5,0.16,74.9,0.868,8.53,1120,0.071);
        int patternA[8] ={0,1,1,0,0,1,1,0};
        int patternB[8] ={0,0,1,1,0,0,1,1};
        ME.SetHallPattern(patternA, patternB,8); // A signal position 0 - N-1, B signal position 0 -N-1 , Number of positions
        ME.AttachHallInterrupt(&Move_interrupt);
        ME.Setup_Interrupts();
        M1_Position = 0;
        M2_Position = 0;

        WHEN("Run with no load at 12v for 10 seconds"){
            ME.Start_Emulation(12,255);
            Sleep(10000);
            ME.Pause_Emulation;
            THEN("Global Position should show an increased value"){
                REQUIRE(M1_Position==1);//calculated number on interrupts ++
            }
        }

        WHEN("Run with no load at 12v for 10 seconds backwards"){
            ME.Start_Emulation(-12,255);
            Sleep(10000);
            ME.Pause_Emulation;
            THEN("Global Position should show an increased value"){
                REQUIRE(M1_Position==1);//calculated number on interrupts ++
            }
        }

        WHEN("Run with no load at 12v for 10 seconds fowards, then 3 backwards"){

            ME.Start_Emulation(12,255);
            Sleep(10000);
            ME.Change_Voltage(-12);
            Sleep(3000);
            ME.Pause_Emulation;
            THEN("Global Position should show an increased value"){
                REQUIRE(M1_Position==1);//calculated number on interrupts ++
            }
        }
    }
}

void Move_interrupt() {// pins D0-7 / 16-23 port change interrupt //On any sensor change start interrupt

        byte pd = PIND; // Read portD
        // Serial.println(pd,BIN);
        byte M1 = pd & M1_Mask; // Mask value for motor checks
        byte M2 = pd & M2_Mask;
        byte x; //Temp space for right shift
        byte y; //Temp space for right shift
        if (M1!=Last_M1_value){  // Test if M1 value has changed
            x = (Last_M1_value >> 2);//Right shift for easy table look up
            y = (M1 >> 2);
            if(M1_Leader=='A')//Which signal = forward;
            {
                if (forward_table[x]==y){//A is leader
                    M1_Position++; //increment Position
                }
                else if(reverse_table[x]==y){//if not a forward change then must be going backwards
                    M1_Position--; //decrement Position
                }

            }
            else if (M1_Leader=='B')
            {
                if (forward_table[x]==y){//A is leader
                    M1_Position--; //decrement Position
                }
                else if(reverse_table[x]==y){
                    M1_Position++; //increment Position
                }

            }
            if(reverse_table[x]==y || forward_table[x]==y){
                M1_Time_Movement=micros();//Take Time of change in microseconds for CLoop
                Motor1Speed[M1Speedindex % MOTORSPEEDARRAYSIZE] = M1_Time_Movement-M1_Time_Last_Movement;
                M1Speedindex++;
                //M1_Speed=M1_Time_Movement-M1_Time_Last_Movement;//Update the Speed of motor
                M1_Time_Last_Movement=M1_Time_Movement;//Store time of this movement
                M1_New_Speed_Flag+=1;//Tell Cloop speed has been updated
                Last_M1_value=M1;//Store last value for noise checking
               /* if((M1_DOM!='S') && (M1_Position == M1_Wanted_Position_as_int)&&(indexing_on != true)){
                    StopMotorClock_M1();
                    M1_AWP=1;
                    M1_Priority_Command=0; //allows new commands if priority command was used for movement.
                    M1_CNS=1;
                    M1_Loop_Last_Time=M1_Loop_Time;
                    M1_Loop_Last_Position=M1_Loop_Position;
                    M1_CStalling=0;
                    M1_Stalling=0;
                    // Clear the state of the control loops
                    M1_Speed_Loop = 0;
                    M1_Current_Loop = 0;
                    M1_bPWM = 0;
                    M1_current_2 = 0;
                    M1_current_1 = 0;
                    M1_DOM='S';
                }*/
            }
            else
            {
                Last_M1_value = M1;
            }

        }

        if (M2!=Last_M2_value){// Test if M2 value has changed
            x=Last_M2_value >> 4;
            y=M2 >> 4;
            if(M2_Leader=='A')//Which signal = forward;
            {
                if (forward_table[x]==y){//A is leader
                    M2_Position++; //increment Position
                }
                else if(reverse_table[x]==y){
                    M2_Position--; //decrement Position
                }

            }
            else if (M2_Leader=='B')
            {
                if (forward_table[x]==y){//A is leader
                    M2_Position--; //decrement Position
                }
                else if(reverse_table[x]==y){
                    M2_Position++; //increment Position
                }

            }
            if(reverse_table[x]==y || forward_table[x]==y){
                M2_Time_Movement=micros();
                Motor2Speed[M2Speedindex % MOTORSPEEDARRAYSIZE] = M2_Time_Movement-M2_Time_Last_Movement;
                M2Speedindex++;
                // M2_Speed=M2_Time_Movement-M2_Time_Last_Movement;
                M2_Time_Last_Movement=M2_Time_Movement;//Store time of this movement
                M2_New_Speed_Flag+=1;
                Last_M2_value=M2;
               /* if((M2_DOM!='S') && (M2_Position == M2_Wanted_Position_as_int)&& (indexing_on != true)){
                    StopMotorClock_M2();
                    M2_AWP=1;
                    M2_Priority_Command=0; //allows new commands if priority command was used for movement.
                    M2_CNS=1;
                    M2_Loop_Last_Time=M2_Loop_Time;
                    M2_Loop_Last_Position=M2_Loop_Position;
                    M2_CStalling=0;
                    M2_Stalling=0;
                    // Clear the state of the control loops
                    M2_Speed_Loop = 0;
                    M2_Current_Loop = 0;
                    M2_bPWM = 0;
                    M2_current_2 = 0;
                    M2_current_1 = 0;
                    M2_DOM='S'; // because there is not motor stop function, loop has to set motor direction as stopped
                }*/
            }
            else
            {
                Last_M2_value = M2;
            }

        }
}



