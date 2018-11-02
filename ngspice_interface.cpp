#include "ngspice_interface.h"
#include <iostream>
#include <fstream>

#include <cstring>
#include <cerrno>
#include <sstream>
#include <string>
#include <sstream>

#include <list>

namespace NGSIF{


bool silence = false;
bool logging_on = true;
bool errorflag = false;
bool no_bg = false;
int no_of_interrupts = 0;
std::ostringstream outputstream;

int
ciprefix(const char *p, const char *s);
int
cieq(register char *p, register char *s);




int
ng_exit(int, bool, bool, int ident, void*);

int
ng_thread_runs(bool noruns, int ident, void* userdata);

int
ng_initdata(pvecinfoall intdata, int ident, void* userdata);

int
ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata);

    int ng_getchar(char* outputreturn, int ident, void* userdata);

    int ng_getstat(char* outputreturn, int ident, void* userdata);

void Ng_spice_set_up(){
    int ret;
    ret = ngSpice_Init(ng_getchar, ng_getstat, ng_exit,  ng_data, ng_initdata, ng_thread_runs, NULL);

};

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
        if(silence == false)
        printf("Vector: %s\n", intdata->vecs[i]->vecname);

        if (logging_on == true){
            char x[100];
            snprintf(x,100,"Vector: %s\n", intdata->vecs[i]->vecname);
            outputstream << std::string(x);
        }

    }
    return 0;
}


/* Callback function called from bg thread in ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int
ng_exit(int exitstatus, bool immediate, bool quitexit, int ident, void* userdata)
{

    if(quitexit) {
        if(silence == false)
        printf("DNote: Returned form quit with exit status %d\n", exitstatus);

        if (logging_on == true){
            char x[100];
            snprintf(x,100,"DNote: Returned form quit with exit status %d\n", exitstatus);
            outputstream << std::string(x);
        }

        exit(exitstatus);
    }
    if(immediate) {
        if(silence == false){
        printf("DNote: Unloading ngspice inmmediately is not possible\n");
        printf("DNote: Can we recover?\n");
        }
        if (logging_on == true) {
            char x[100];
            snprintf(x, 100, "DNote: Unloading ngspice inmmediately is not possible\n DNote: Can we recover?\n");
            outputstream << std::string(x);
        }
    }

    else {
        if (silence == false){
            printf("DNote: Unloading ngspice is not possible\n");
        printf("DNote: Can we recover? Send 'quit' command to ngspice.\n");
        }
        if (logging_on == true) {
            char x[100];
            snprintf(x, 100, "DNote: Unloading ngspice is not possible\n DNote: Can we recover? Send 'quit' command to ngspice.\n");
            outputstream << std::string(x);
        }

        errorflag = true;
        ngSpice_Command((char*)"quit 5");
//        raise(SIGINT);
    }

    return exitstatus;
}
/* Callback function called from bg thread in ngspice to transfer
any string created by printf or puts. Output to stdout in ngspice is
preceded by token stdout, same with stderr.*/
int ng_getchar(char* outputreturn, int ident, void* userdata)
{
    if(silence == false)
        printf("%s\n", outputreturn);

    /* setting a flag if an error message occurred */
    if (ciprefix("stderr Error:", outputreturn))
        errorflag = true;//do not throw here use to throw in motor emulator.

    if (logging_on == true)
        outputstream << std::string(outputreturn) << '\n';

    return 0;
}


int ng_getstat(char* outputreturn, int ident, void* userdata)
{
    if(silence == false)
        printf("%s\n", outputreturn);

    if (logging_on == true)
        outputstream << std::string(outputreturn) << '\n';

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



}

