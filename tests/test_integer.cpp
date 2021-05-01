#include "../utils/integer/integer.h"
#include "test_utils.h"
#include <iostream>
#include <typeinfo>

auto test_basic_integer() -> bool {
  bool passed = true;
  Integer<4> small{};
  small = 3;
  REQUIRE_EQUAL(small, 3);
  small = 0xFF;
  REQUIRE_EQUAL(small, 0xF);

  Integer<12> not_as_small{};
  not_as_small = 0b101010101101;
  REQUIRE_EQUAL(not_as_small.bit(0), 1);
  REQUIRE_EQUAL(not_as_small.bit(1), 0);

  Integer<16> convert = small;
  REQUIRE_EQUAL(convert, 0xF);
  return true;
}

auto test_integer() -> bool {
  bool passed = true;
  RUN_TEST(test_basic_integer());
  return passed;
}
