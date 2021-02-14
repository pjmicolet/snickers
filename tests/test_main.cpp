#include <iostream>
#include "test_utils.h"

extern bool test_ram();
extern bool test_rom();
extern bool test_typeutils();
extern bool test_integer();

int main() {
    bool passed = true;
    RUN_TEST( test_ram() );
    RUN_TEST( test_rom() );
    RUN_TEST( test_typeutils() );
    RUN_TEST( test_integer() );
    return 0;
}
