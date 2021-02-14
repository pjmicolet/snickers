#include "../core/memory.h"
#include <iostream>
#include "test_utils.h"

bool test_basic() {
	std::vector< int > trace = { 1 };
	RAM ram = RAM( 10, trace );
	ram.store( 1, 120 );
	
	if( ram[ 1 ] != 120 )
		return false;
	return true;
}

bool test_ram() {
	bool passed = true;
	RUN_TEST( test_basic() );
	return passed;
}

