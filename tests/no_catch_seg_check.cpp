//
// Created by TCT on 30/07/2018.
//
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../ngspice_interface.h"

std::vector<std::string> stringTOlines(std::string Ostream);
int main()
{
    /*NGSIF::*/Ng_spice_set_up();
    /*NGSIF::*/silence = false;
    /*NGSIF::*/logging_on = true;
    ngSpice_Command((char*)"version");

    auto x = /*NGSIF::*/outputstream.str();
    auto lines1 = stringTOlines(x);
    auto ritti = lines1.rbegin();
    ritti;
    std::string p = (*ritti);
    std::cout << "line 0 ==: " << (*ritti).c_str() << std::endl;

    ngSpice_Command((char*)"source C:\\Users\\TCT\\Dropbox\\Hand_code_Advance\\Motor_Emulator\\test_circuts\\rfswitch.cir");
    auto xx = /*NGSIF::*/outputstream.str();
    auto lines2 = stringTOlines(xx);
    auto ritt = lines2.rbegin();
    ritt += 2;
    std::string pp = (*ritt);
    std::cout << "line 1 ==: " << (*ritt).c_str() << std::endl;

    return 0;
}


std::vector<std::string> stringTOlines(std::string Ostream){
    std::string str = Ostream;
    std::stringstream ss(str);
    std::vector<std::string> lines;
    while(std::getline(ss,str,'\n')){
        lines.push_back(str);
    }
    return lines;
}

