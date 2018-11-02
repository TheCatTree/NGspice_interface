//
// Created by TCT on 6/07/2018.
//
#include <windows.h>
#include <synchapi.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include "catch.hpp"
#include "../System_Timer.h"
#include "MockArduino.h"


// Custom Matcher for unsigned longs for a +/- value range.
// The matcher class
class uLongRange : public Catch::MatcherBase<unsigned long> {
    long long m_value;
    int m_plus, m_minus;
public:
    uLongRange(unsigned long value ,unsigned int plus, unsigned int minus) : m_value(value), m_plus(plus), m_minus(minus) {}

    // Performs the test for this matcher
    virtual bool match(unsigned long const& i ) const override {
        return i >= m_value - m_minus && i <= m_value + m_plus;
    }

    // Produces a string describing what this matcher does. It should
    // include any provided data (the begin/ end in this case) and
    // be written as if it were stating a fact (in the output it will be
    // preceded by the value under test).
    virtual std::string describe() const {
        std::ostringstream ss;
        ss << "is between " << m_value - m_minus  << " and " << m_value + m_plus;
        return ss.str();
    }
};

// The builder function
inline uLongRange Isnear( unsigned long v,int p, int m ) {
    return uLongRange( v,p,m );
}

SCENARIO("A single System Timer being compared to mock arduino timer", "[Timer]"){
    GIVEN("A single timer reset"){
        system_timer timer;
        timer.reset();
        setMockarduinoclockstartpoint(std::chrono::high_resolution_clock::now());
        Sleep(1000);
        WHEN("Arduino times are returned and compered"){
            auto mill_1 = millis();
            auto micro_1 = micros();
            Sleep(1000);
            auto mill_2 = millis();
            auto micro_2 = micros();
            THEN("discrepancies in time should be obvious"){
                CHECK(mill_1 == micro_1/1000);
                CHECK(mill_2 == micro_2/1000);
                auto mill_delta = mill_2-mill_1;
                auto micro_delta = micro_2 -micro_1;
                REQUIRE(mill_delta == micro_delta/1000);
            }
        }
        WHEN("Ran for 1 second"){
            timer.start();
            auto x = micros();
            Sleep(1000);
            timer.stop();
            x =  micros() - x;
            THEN("return a count of 1,000,000 microseconds"){
                REQUIRE_THAT(timer.get_time(), Isnear(x,5,5));

            }
        }
    }

}