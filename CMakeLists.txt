cmake_minimum_required(VERSION 3.10)
project(Motor_Emulator)

set(CMAKE_CXX_STANDARD 11)

add_library(Motor_Emulator library.cpp library.h tests/catch.hpp)
add_executable(utests tests/main_tests.cpp tests/Test_Motor_Emulator.cpp)