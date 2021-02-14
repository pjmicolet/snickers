#include <iostream>
#include "test_utils.h"

extern bool test_ram();
extern bool test_rom();

int main() {
	bool passed = true;
	RUN_TEST( test_ram() );
	RUN_TEST( test_rom() );
	return 0;
}
