cmake_minimum_required(VERSION 3.10)
project(Motor_Emulator)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
#SET(CMAKE_AR  "gcc-ar")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
set(CMAKE_VERBOSE_MAKEFILE ON)
enable_testing()
include_directories( ${MY_SOURCE_DIR}/tests )
include_directories( ${MY_SOURCE_DIR}/lib/Spice64_dll/include/ngspice)
include_directories(${MY_SOURCE_DIR}/lib/Spice64_dll/lib)
include_directories(${MY_SOURCE_DIR}/lib/Spice64_dll/bin-vs)

add_library(ngspice STATIC IMPORTED)
set_property(TARGET ngspice PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/Spice64_dll/lib/ngspice.lib)

add_library(test_main OBJECT tests/main_tests.cpp)
add_library(mock_arduino tests/MockArduino.cpp tests/MockArduino.h tests/ngSpiceInterface_tests.cpp)
add_library(System_Timer OBJECT System_Timer.cpp System_Timer.h)
add_library(logger OBJECT logger.h)
add_library(NGSpice_interface ngspice_interface.h ngspice_interface.cpp)

add_custom_command(
        TARGET NGSpice_interface
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/motors
        ${CMAKE_CURRENT_BINARY_DIR}/motors
)

add_custom_command(
        TARGET NGSpice_interface
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/test_circuts
        ${CMAKE_CURRENT_BINARY_DIR}/test_circuts
)

target_link_libraries(NGSpice_interface  ngspice)

add_executable(System_Timer_test tests/System_Timer_tests.cpp System_Timer.cpp System_Timer.h tests/main_tests.cpp)
target_link_libraries(System_Timer_test mock_arduino)
add_test(NAME System_Timer_test  COMMAND System_Timer_test)

add_executable(Test_Motor_Emulator_test tests/Test_Motor_Emulator.cpp  $<TARGET_OBJECTS:test_main>)
add_test(NAME Test_Motor_Emulator_test  COMMAND Test_Motor_Emulator_test)

add_executable(ngspice_interface_test tests/ngSpiceInterface_tests.cpp $<TARGET_OBJECTS:test_main> )
target_link_libraries(ngspice_interface_test NGSpice_interface)
add_test(NAME ngspice_interface_test  COMMAND ngspice_interface_test)

add_executable(no_catch_seg_check tests/no_catch_seg_check.cpp)
target_link_libraries(no_catch_seg_check NGSpice_interface)

add_executable(catch_example tests/catch_example.cpp $<TARGET_OBJECTS:test_main>)

add_library(library library.h library.cpp)
target_link_libraries(library ngspice)

