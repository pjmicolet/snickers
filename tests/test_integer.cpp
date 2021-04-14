#include "../utils/integer/integer.h"
#include "test_utils.h"
#include <iostream>

auto test_basic_integer() -> bool
{
	Integer<15> a{};
	a = 10;
	Integer<17> b{};
	b = 131071;
	a = b;
	return true;
}
    
auto test_integer() -> bool
{
    bool passed = true;
    RUN_TEST( test_basic_integer() );
    return passed;
}
