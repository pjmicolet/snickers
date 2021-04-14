#include "../core/rom.h"
#include <iostream>
#include "test_utils.h"

auto test_basic_load() -> bool {
    Rom rom( "./Ladida_lol.sfc");    

    if( rom.size() == 0 )
        return false;

    return true;
}

auto test_wrong_path() -> bool {
    bool pass = false;
    try {
        Rom rom( "nowhereland" );
    } catch ( RomException& e ) {
        pass = true;
    }
    
    return pass;
}

auto test_rom() -> bool {
    bool passed = true;
    RUN_TEST( test_basic_load() );
    RUN_TEST( test_wrong_path() );
    return passed;
}
