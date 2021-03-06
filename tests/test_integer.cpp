#include "../utils/integer/integer.h"
#include "test_utils.h"
#include <iostream>
#include <typeinfo>

auto test_basic_integer() -> bool {
  bool passed = true;
  constexpr Integer<4> small = 3;
  STATIC_REQUIRE_EQUAL(small, 3);
  constexpr Integer<4> small2 = 0xFF;
  STATIC_REQUIRE_EQUAL(small2, 0xF);

  constexpr Integer<12> not_as_small = 0b101010101101;
  STATIC_REQUIRE_EQUAL(not_as_small.bit(0), 1);
  STATIC_REQUIRE_EQUAL(not_as_small.bit(1), 0);

  constexpr Integer<16> convert = small2;
  STATIC_REQUIRE_EQUAL(convert, 0xF);
  return passed;
}

auto test_integer() -> bool {
  bool passed = true;
  RUN_TEST(test_basic_integer());
  return passed;
}
