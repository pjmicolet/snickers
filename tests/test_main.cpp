#include "test_utils.h"
#include <iostream>

extern bool test_ram();
extern bool test_rom();
extern bool test_typeutils();
extern bool test_integer();
extern bool test_nes();
extern bool test_asm();
extern bool test_container_utils();

int main() {
  bool passed = true;
  RUN_TEST(test_ram());
  RUN_TEST(test_rom());
  RUN_TEST(test_typeutils());
  RUN_TEST(test_integer());
  RUN_TEST(test_nes());
  RUN_TEST(test_asm());
  RUN_TEST(test_container_utils());
  return passed;
}
