#include "library.h"
#include "tests/catch.hpp"
#include <fstream>
#include <string>
#include <cstring>
#include <cerrno>
#include <sstream>
#include <iostream>
#include <list>

bool errorflag = false;
bool no_bg = false;
int no_of_interrupts = 0;
int
ciprefix(const char *p, const char *s);
int
cieq(register char *p, register char *s);


void Motor_Emulator::PORTD_Interrupt() {
    raise(SIGINT);
}

void Motor_Emulator::Start_Emulation(int Voltage, int PWM) {

}

void Motor_Emulator::SetHallPattern(std::vector<int> &A_pattern, std::vector<int> &B_pattern) {

       this->A_Hall_pattern = A_pattern;
        this->B_Hall_pattern = B_pattern;

}

void Motor_Emulator::Pause_Emulation() {

}

void Motor_Emulator::Stop_Emulation() {

}

void Motor_Emulator::Place_Load_on_motor() {

}

void Motor_Emulator::Change_Voltage(int Voltage) {

}

void Motor_Emulator::Setup_Interrupts() {

}

int
ng_getchar(char* outputreturn, int ident, void* userdata);

int
ng_getstat(char* outputreturn, int ident, void* userdata);

int
ng_exit(int, bool, bool, int ident, void*);

int
ng_thread_runs(bool noruns, int ident, void* userdata);

int
ng_initdata(pvecinfoall intdata, int ident, void* userdata);

int
ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata);



void Ng_spice_set_up(){
    int ret;
    ret = ngSpice_Init(ng_getchar, ng_getstat, ng_exit,  ng_data, ng_initdata, ng_thread_runs, NULL);

};
std::list<std::string> Ng_spice_load_net(const char *filename);

std::list<std::string> Ng_spice_load_net(const char *filename) {
    int ret;
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        std::list<std::string> command_set;
        std::string line;
        std::istringstream f(contents);
        while (std::getline(f, line)) {
            command_set.push_back(line);
        }
        return(command_set);
    }
    throw(errno);
}
double Ng_spice_run(std::list<std::string> &commandset);

double Ng_spice_run(std::list<std::string> &commandset) {
    int ret;
    //Loading net
    std::string circ ="circbyline " ;
    std::string out;
    std::list<std::string>::iterator i;
    for( i = commandset.begin(); i!= commandset.end(); ++i){
        out = circ + *i;
        auto * cstr = new char [out.length()+1];
        std::strcpy(cstr, out.c_str());
        ret = ngSpice_Command(cstr);
        printf("%s \n",cstr);
        delete[] cstr;
    }
    ret = ngSpice_Command((char*)"options ACCT");
   // ret = ngSpice_Command((char*)"options opts");
    unsigned long time = micros();
    printf("run started \n");
    ret = ngSpice_Command((char*)"run");
    printf("run was %d ms long \n",(int)( micros() - time ) );
    return ((double)time/1000000.0);
}
void Ng_spice_print_last_run();
void Ng_spice_print_last_run(){
    char** allplots = ngSpice_AllPlots();
    char **vecnames;
    pvector_info vecinfo;
    int sizeofvec = 0;
    for (auto x = 0; allplots[x]!=NULL; x++ ){
        printf("Plot name: %s \n", allplots[x]);
    }
    char* plotname = ngSpice_CurPlot();
        printf("Cur plot name: %s \n", plotname);
        vecnames = ngSpice_AllVecs(plotname);
        for (auto i = 0; vecnames[i]!=NULL; i++) {
            printf("%s \n", vecnames[i]);
            vecinfo = ngGet_Vec_Info(vecnames[i]);
            sizeofvec = vecinfo->v_length;
            printf("%d \n", sizeofvec);
            printf("Type: %d \n", vecinfo->v_type);
            printf("value of first, 2 ,10, 100 ,middle, last: %f ,%f ,%f ,%f ,%f ,%f\n", vecinfo->v_realdata[0],vecinfo->v_realdata[1],vecinfo->v_realdata[9],vecinfo->v_realdata[99],
                   vecinfo->v_realdata[sizeofvec / 2 -1], vecinfo->v_realdata[sizeofvec - 1]);
        }
    }
Time_line Ng_spice_get_last_run();
Time_line Ng_spice_get_last_run(){
    char **vecnames;
    pvector_info vecinfo;
    int sizeofvec = 0;
    Time_line  out;
    char* plotname = ngSpice_CurPlot();
    printf("Cur plot name: %s \n", plotname);
    vecnames = ngSpice_AllVecs(plotname);
    for (auto i = 0; vecnames[i]!=NULL; i++) {
        vecinfo = ngGet_Vec_Info(vecnames[i]);
        out.v_info[vecinfo->v_name]=*vecinfo;
    }
    out.Time_line_length_points = out.v_info["time"].v_length;
    out.Time_line_length_S = out.v_info["time"].v_realdata[out.Time_line_length_points-1];
    return out;

};
std::list<std::string> Ng_command_edit(std::list<std::string> &command, std::string &find, std::string &replacement);
std::list<std::string> Ng_command_edit(std::list<std::string> &command, std::string &find, std::string &replacement){
    std::list<std::string> commands = command;
    std::list<std::string>::iterator c;
    for( c = commands.begin(); c != commands.end(); ++c ){
            std::string str = *c;
        if(str.find(find) != std::string::npos){ (*c)=replacement;break;}
    }
    return commands;
}

std::string Ng_PWM_Pulse_command(int Hz, int dutyCycle, int voltage);
std::string Ng_PWM_Pulse_command(int Hz, int dutyCycle, int voltage){
    double TotaluS = 1000000.0 / (double)Hz;
    double OnuS = (TotaluS / 100.0) * (double)dutyCycle;
    char buff[256];
    snprintf(buff, sizeof(buff),"V_drive vtop 0 PULSE(0 %d 0 180N 160N %fU %fU)", voltage , OnuS, TotaluS);
    std::string out = buff;
    return out;

};
std::list<std::string> Ng_Change_PWM(std::list<std::string> &command,int Hz, int dutyCycle, int voltage);
std::list<std::string> Ng_Change_PWM(std::list<std::string> &command,int Hz, int dutyCycle, int voltage){
    std::string x ="PULSE(";
    std::string y = Ng_PWM_Pulse_command(Hz, dutyCycle,voltage);
    return Ng_command_edit(command,x,y);
}


std::list<std::string> ArduinoStylePWM(byte PWM,std::list<std::string> &command);
std::list<std::string> ArduinoStylePWM(byte PWM,std::list<std::string> &command){
    return Ng_Change_PWM(command,62500,(int)(100.0/256.0*(float)PWM),12);
}

std::list<std::string> Ng_command_inital_values(std::list<std::string> command, Time_line &line, long line_position);


std::list<std::string> Ng_command_inital_values(std::list<std::string> command, Time_line &line, long line_position){
    //Clean out any original .ic command
    for( auto c = command.begin(); c != command.end(); ++c ){
        std::string str = *c;
        if(str.find(".ic") != std::string::npos){ command.erase(c);break;}
    }
//Get data Values
    double vtop = line.v_info["vtop"].v_realdata[line_position];
    double n001 = line.v_info["n001"].v_realdata[line_position];
    double n002 = line.v_info["n002"].v_realdata[line_position];
    double n003 = line.v_info["n003"].v_realdata[line_position];
    double n004 = line.v_info["n004"].v_realdata[line_position];
    double n005 = line.v_info["n005"].v_realdata[line_position];
    double n006 = line.v_info["n006"].v_realdata[line_position];
    double n007 = line.v_info["n007"].v_realdata[line_position];
    double p001 = line.v_info["p001"].v_realdata[line_position];
    double angularsum = line.v_info["angularsum"].v_realdata[line_position];

    double v_sense_1 = line.v_info["v_sense_1#branch"].v_realdata[line_position];
    double V_SENSE_2 = line.v_info["v_sense_2#branch"].v_realdata[line_position];
    double L_Motor = line.v_info["l_motor#branch"].v_realdata[line_position];
    double Lrotor_Inertia = line.v_info["lbrotor_inertia#branch"].v_realdata[line_position];

    //setting voltage intial values
    char buff[512];
    snprintf(buff, sizeof(buff),".ic "
                                "V(vtop)=%f "
                                "V(N001)=%f "
                                "V(N001)=%f "
                                "V(N003)=%f "
                                "V(N004)=%f "
                                "V(N005)=%f "
                                "V(N006)=%f "
                                "V(N007)=%f "
                                "V(P001)=%f "
                                "V(AngularSum)=%f ",
                                 vtop,
             n001,
             n002,
             n003,
             n004,
             n005,
             n006,
             n007,
             p001,
             angularsum
    );
    std::string ic = buff;
    for( auto c = command.begin(); c != command.end(); ++c ){
        std::string str = *c;
        if(str.find(".end") != std::string::npos){ command.insert(c,ic);break;}
    }
    /*"I(V_SENSE_1)=%f "
    "I(V_SENSE_2)=%f "
    "I(L_Motor)=%f "
    "I(rotor_Inertia)=%f ",*/
    /*,v_sense_1,V_SENSE_2,L_Motor,Lrotor_Inertia*/
    //IC on inductors
    for( auto c = command.begin(); c != command.end(); ++c ){
        std::string str (*c);
        if(str.find("L_Motor") != std::string::npos){
            std::string tstr (*c) ;
            //clean string
            tstr.erase(std::remove(tstr.begin(),tstr.end(), '\r'),tstr.end());
            tstr += " ic=";
            tstr += std::to_string(L_Motor);
            printf("%s \n", tstr.c_str());
            std::cout << "String length: " << str.size();
            std::cout << str ;
            std::cout << tstr<<"\n";
            *c = tstr;

            }
        if(str.find("rotor_Inertia") != std::string::npos){
            std::string tstr = *c;
            //clean string
            tstr.erase(std::remove(tstr.begin(),tstr.end(), '\r'),tstr.end());
            tstr += " ic=";
            tstr += std::to_string(Lrotor_Inertia);
            printf("%s \n", tstr.c_str());
            *c = tstr;
        }
    }

    return command;
}


/*************************/
/*  UNIT TESTS          */
/************************/

TEST_CASE( "NG Spice asynchronous data structure" , "[NG Asynch]")

{

    Ng_spice_set_up();
   auto x = Ng_spice_load_net("C:\\Users\\TCT\\Dropbox\\Hand_code_Advance\\Motor_Emulator\\motors\\DSX10smotor.net");
    x = ArduinoStylePWM(240,x);
   auto time_start = Ng_spice_run(x);
    Ng_spice_print_last_run();
    auto runinfo = Ng_spice_get_last_run();
    runinfo.Start_Time_S = time_start;
    //print all map, keys and value
    for (auto i : runinfo.v_info){
        std::string tmp_str = i.first;
        int tmp_int= i.second.v_length;
        double tmp_doub = i.second.v_realdata[(tmp_int/2) -1];
        double tmp_doub_2 = i.second.v_realdata[tmp_int -1];
        printf("Map: Name: %s, Middle real value: %f , Middle last value %f \n", tmp_str.c_str(),tmp_doub, tmp_doub_2);
    }
    //set .ic to runinfo values then run with new pwm and print.
    x = Ng_command_inital_values(x,runinfo,runinfo.Time_line_length_points -1);
    time_start = Ng_spice_run(x);
    Ng_spice_print_last_run();
}

/* Callback function called from bg thread in ngspice once upon intialization
   of the simulation vectors)*/
int
ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata)
{
    int *ret;
    no_of_interrupts ++;
    //printf("No of ng data interrupts: %d \n", no_of_interrupts);
    return 0;
}
int
ng_initdata(pvecinfoall intdata, int ident, void* userdata)
{
    int i;
    int vn = intdata->veccount;
    for (i = 0; i < vn; i++) {
        printf("Vector: %s\n", intdata->vecs[i]->vecname);
    }
    return 0;
}


/* Callback function called from bg thread in ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int
ng_exit(int exitstatus, bool immediate, bool quitexit, int ident, void* userdata)
{

    if(quitexit) {
        printf("DNote: Returned form quit with exit status %d\n", exitstatus);
        exit(exitstatus);
    }
    if(immediate) {
        printf("DNote: Unloading ngspice inmmediately is not possible\n");
        printf("DNote: Can we recover?\n");
    }

    else {
        printf("DNote: Unloading ngspice is not possible\n");
        printf("DNote: Can we recover? Send 'quit' command to ngspice.\n");
        errorflag = true;
        ngSpice_Command((char*)"quit 5");
//        raise(SIGINT);
    }

    return exitstatus;
}
/* Callback function called from bg thread in ngspice to transfer
any string created by printf or puts. Output to stdout in ngspice is
preceded by token stdout, same with stderr.*/
int
ng_getchar(char* outputreturn, int ident, void* userdata)
{
    printf("%s\n", outputreturn);
    /* setting a flag if an error message occurred */
    if (ciprefix("stderr Error:", outputreturn))
        errorflag = true;
    return 0;
}


int
ng_getstat(char* outputreturn, int ident, void* userdata)
{
    printf("%s\n", outputreturn);
    return 0;
}

int
ng_thread_runs(bool noruns, int ident, void* userdata)
{
    no_bg = noruns;
    if (noruns)
        printf("bg not running\n");
    else
        printf("bg running\n");

    return 0;
}

int ciprefix(const char *p, const char *s) {
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) != //clever
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (true);
}

int cieq(register char *p, register char *s) {
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (*s ? false : true);
}



/* Case insensitive prefix. */
