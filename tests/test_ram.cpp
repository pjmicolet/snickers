#include "../core/memory.h"
#include <iostream>
#include "test_utils.h"

bool test_basic() {
    bool passed = true;
    std::vector< int > trace = { 1 };
    RAM ram = RAM( 10, trace );
    ram[1] = 120;
    
    passed = ( ram[1] == 120 );
    
    ram[2] = ram[1];

    passed = ( ram[2] == 120 );
    passed = ( ram[1] != ram[3] );
    passed = ( ram[1] == ram[2] );
    
    return passed;
}

bool test_ram() {
    bool passed = true;
    RUN_TEST( test_basic() );
    return passed;
}

