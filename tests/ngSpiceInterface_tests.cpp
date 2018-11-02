//
// Created by TCT on 30/07/2018.
//
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "../ngspice_interface.h"
void Ng_spice_print_last_run();
std::vector<std::string> stringTOlines(std::string Ostream);
SCENARIO("Running basic spice commands threw api interface", "[Spice Interface]") {
    GIVEN("the basic spice interface commands") {

        NGSIF::Ng_spice_set_up();
        NGSIF::silence = false;
        NGSIF::logging_on = true;

        WHEN("Info command is sent") {
            ngSpice_Command((char *) "version");

            auto x = NGSIF::outputstream.str();
            auto lines1 = stringTOlines(x);
            auto ritti = lines1.rbegin();
            ritti;
            std::string p = (*ritti);
            //std::cout << "line 0 ==: " << (*ritti).c_str() << std::endl;

            THEN("debug log should show known txt.") {


                REQUIRE((*ritti).compare("stdout ******") == 0);
            }
        }

        WHEN("Circuit is loaded") {
            ngSpice_Command(
                    (char *) "source test_circuts\\rfswitch.cir");
            auto x = NGSIF::outputstream.str();
            auto lines2 = stringTOlines(x);
            auto ritt = lines2.rbegin();
            ritt += 2;
            std::string pp = (*ritt);
            //std::cout << "line 1 ==: " << (*ritt).c_str() << std::endl;

            THEN("Circuit is loaded") {

                REQUIRE((*ritt).compare("stdout Circuit: *** rf switch circuit ***") == 0);
            }
        }

        WHEN("Circuit is loaded and run") {
            ngSpice_Command(
                    (char *) "source motors\\DSX10smotor.net");
            ngSpice_Command((char *) "run");
            //auto x = NGSIF::outputstream.str();
            //auto lines2 = stringTOlines(x);
            //auto ritt = lines2.rbegin();
            //ritt += 2;
            //std::string pp = (*ritt);
            //std::cout << "line 1 ==: " << (*ritt).c_str() << std::endl;
            //Ng_spice_print_last_run();
            //char* plotname = ngSpice_CurPlot();
            vector_info vecinfo_angularsum = *ngGet_Vec_Info((char *) "angularsum");
            vector_info vecinfo_back_emf = *ngGet_Vec_Info((char *) "back_emf#branch");
            vector_info vecinfo_v_drive = *ngGet_Vec_Info((char *) "v_drive#branch");
            vector_info vecinfo_lbrotor_inertia = *ngGet_Vec_Info((char *) "lb\'rotor_inertia#branch");

            auto sizeofvec = vecinfo_angularsum.v_length;
           //printf(" %f\n", vecinfo_angularsum.v_realdata[sizeofvec - 1]);

            sizeofvec = vecinfo_back_emf.v_length;
            //printf(" %f\n", vecinfo_back_emf.v_realdata[sizeofvec - 1]);

            sizeofvec = vecinfo_v_drive.v_length;
            //printf(" %f\n", vecinfo_v_drive.v_realdata[sizeofvec - 1]);

            sizeofvec = vecinfo_lbrotor_inertia.v_length;
            //printf(" %f\n", vecinfo_lbrotor_inertia.v_realdata[sizeofvec - 1]);

            THEN("Circuit is run") {
                sizeofvec = vecinfo_angularsum.v_length;
                REQUIRE(vecinfo_angularsum.v_realdata[sizeofvec - 1] == Approx(162.719876).epsilon(0.01));

                sizeofvec = vecinfo_back_emf.v_length;
                REQUIRE(vecinfo_back_emf.v_realdata[sizeofvec - 1] == Approx(-0.020891).epsilon(0.01));

                sizeofvec = vecinfo_v_drive.v_length;
                REQUIRE(vecinfo_v_drive.v_realdata[sizeofvec - 1] == Approx(0.020891).epsilon(0.01));

                sizeofvec = vecinfo_lbrotor_inertia.v_length;
                REQUIRE(vecinfo_lbrotor_inertia.v_realdata[sizeofvec - 1] == Approx(669.081562).epsilon(0.01));
                //REQUIRE((*ritt).compare("stdout Circuit: *** rf switch circuit ***")==0);
            }
        }
            WHEN("Circuit is loaded and run from old state") {
                ngSpice_Command(
                        (char *) "source motors\\DSX10smotorUIC.net");
                ngSpice_Command((char *) "run");
                //auto x = NGSIF::outputstream.str();
                //auto lines2 = stringTOlines(x);
                //auto ritt = lines2.rbegin();
                //ritt += 2;
                //std::string pp = (*ritt);
                //std::cout << "line 1 ==: " << (*ritt).c_str() << std::endl;
                //Ng_spice_print_last_run();
                //char* plotname = ngSpice_CurPlot();
                vector_info vecinfo_angularsum = *ngGet_Vec_Info((char *) "angularsum");
                vector_info vecinfo_back_emf = *ngGet_Vec_Info((char *) "back_emf#branch");
                vector_info vecinfo_v_drive = *ngGet_Vec_Info((char *) "v_drive#branch");
                vector_info vecinfo_lbrotor_inertia = *ngGet_Vec_Info((char *) "lb\'rotor_inertia#branch");

                auto sizeofvec = vecinfo_angularsum.v_length;
                //printf(" %f\n", vecinfo_angularsum.v_realdata[sizeofvec - 1]);

                sizeofvec = vecinfo_back_emf.v_length;
                //printf(" %f\n", vecinfo_back_emf.v_realdata[sizeofvec - 1]);

                sizeofvec = vecinfo_v_drive.v_length;
                //printf(" %f\n", vecinfo_v_drive.v_realdata[sizeofvec - 1]);

                sizeofvec = vecinfo_lbrotor_inertia.v_length;
                //printf(" %f\n", vecinfo_lbrotor_inertia.v_realdata[sizeofvec - 1]);

                THEN("Circuit is run") {
                    sizeofvec = vecinfo_angularsum.v_length;
                    REQUIRE(vecinfo_angularsum.v_realdata[sizeofvec - 1] == Approx(329.994482).epsilon(0.01));

                    sizeofvec = vecinfo_back_emf.v_length;
                    REQUIRE(vecinfo_back_emf.v_realdata[sizeofvec - 1] == Approx(-0.020891).epsilon(0.01));

                    sizeofvec = vecinfo_v_drive.v_length;
                    REQUIRE(vecinfo_v_drive.v_realdata[sizeofvec - 1] == Approx(0.020891).epsilon(0.01));

                    sizeofvec = vecinfo_lbrotor_inertia.v_length;
                    REQUIRE(vecinfo_lbrotor_inertia.v_realdata[1] == Approx(669.081562).epsilon(0.01));
                    //REQUIRE((*ritt).compare("stdout Circuit: *** rf switch circuit ***")==0);
                }
            }

        }

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