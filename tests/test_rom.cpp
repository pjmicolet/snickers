#include "../core/rom.h"
#include <iostream>
#include "test_utils.h"

bool test_basic_load() {
    Rom rom( "./Ladida_lol.sfc");    

    if( rom.size() == 0 )
        return false;

    return true;
}

bool test_rom() {
    bool passed = true;
    RUN_TEST( test_basic_load() );
    return passed;
}
