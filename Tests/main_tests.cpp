//
// Created by TCT on 15/04/2018.
//
#define CATCH_CONFIG_RUNNER  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

int main( int argc, char* argv[]){
    int result = Catch::Session().run(argc, argv);
    return result;
}
