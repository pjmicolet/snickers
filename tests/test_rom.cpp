#include "../core/rom.h"
#include <iostream>
#include "test_utils.h"

bool test_basic_load() {
    Rom rom( "./Ladida_lol.sfc");    

    if( rom.size() == 0 )
        return false;

    return true;
}

bool test_wrong_path() {
    bool pass = false;
    try {
        Rom rom( "nowhereland" );
    } catch ( RomException& e ) {
        pass = true;
    }
    
    return pass;
}

bool test_rom() {
    bool passed = true;
    RUN_TEST( test_basic_load() );
    RUN_TEST( test_wrong_path() );
    return passed;
}
