#include "../core/memory.h"
#include <iostream>
#include "test_utils.h"

bool test_basic() {
    // Only a single bank, 10 lines
    bool passed = true;
    RAM ram = RAM( 10 );
    ram[1] = std::byte{ 120 };
    
    passed &= ( ram[1] == std::byte{ 120 } );
    
    ram[2] = ram[1];

    passed &= ( ram[2] == std::byte{ 120 } );
    passed &= ( ram[1] != ram[3] );
    passed &= ( ram[1] == ram[2] );
    
    return passed;
}

bool test_banks() {
    bool passed = true;
    RAM ram = RAM( 200, 2 ); // That's 2 banks each with 100 lines;
    // We run this in debug mode which will do a check 
#if DEBUG
    try {
        ram[300] = std::byte{ 69 };
        passed &= false;
    }
    catch( RamException& msg ) {
        std::cout << "Threw ! " << msg.what() << "\n";
    }
#endif

    ram[ 0x000019 ] = std::byte{ 10 }; // Bank 0 line 25
    ram[ 0x010032 ] = std::byte{ 10 }; // Bank 1 line 50
    ram[ 0x010019 ] = std::byte{ 11 }; // Bank 1 line 50

    passed &= ram[ 0x000019 ] == ram[ 0x010032 ];
    passed &= ram[ 0x000019 ] != ram[ 0x010019 ]; // Just make sure banks are being read correctly
    passed &= ram[ 0x010000 ].bank_ == 1; // better way to do it
    return passed;
}

bool test_ram() {
    bool passed = true;
    RUN_TEST( test_basic() );
    RUN_TEST( test_banks() );
    return passed;
}

