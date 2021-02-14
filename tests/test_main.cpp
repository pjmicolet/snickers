#include <iostream>
#include "test_utils.h"

extern bool test_ram();
extern bool test_rom();
extern bool test_typeutils();

int main() {
    bool passed = true;
    RUN_TEST( test_ram() );
    RUN_TEST( test_rom() );
    RUN_TEST( test_typeutils() );
    return 0;
}
