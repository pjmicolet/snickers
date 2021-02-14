#include "../utils/integer/integer.h"
#include "test_utils.h"
#include <iostream>

bool test_basic_integer()
{
	Integer<15> a{};
	a = 10;
	Integer<17> b{};
	b = 131071;
	a = b;
	return true;
}
    
bool test_integer()
{
    bool passed = true;
    RUN_TEST( test_basic_integer() );
    return passed;
}
