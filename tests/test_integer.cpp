#include "../utils/integer/integer.h"
#include "test_utils.h"
#include <iostream>
#include <typeinfo>

auto test_basic_integer() -> bool {
  bool passed = true;
  constexpr Integer<4> small = 3;
  STATIC_REQUIRE_EQUAL(small, (int8_t)3);
  constexpr Integer<4> small2 = 0xFF;
  STATIC_REQUIRE_EQUAL(small2, 0xF);

  constexpr Integer<12> not_as_small = 0b101010101101;
  STATIC_REQUIRE_EQUAL(not_as_small.bit(0), 1);
  STATIC_REQUIRE_EQUAL(not_as_small.bit(1), 0);

  constexpr Integer<16> convert = small2;
  STATIC_REQUIRE_EQUAL(convert, 0xF);

  Integer<17> lhs = 123;
  Integer<18> rhs = 456;
  Integer<18> sum = lhs + rhs;

  Integer<18> sum2;
  // The real test is to check this compiles!
  sum2 = lhs + rhs;

  REQUIRE_EQUAL(sum, 579);
  REQUIRE_EQUAL(sum.bit(0), 1);
  REQUIRE_EQUAL(sum, sum2);
  return passed;
}

auto test_basic_unsigned() -> bool {
  bool passed = true;
  constexpr Unsigned<4> small = 3;
  STATIC_REQUIRE_EQUAL(small, (uint8_t)3);
  constexpr Unsigned<4> small2 = 0xFF;
  STATIC_REQUIRE_EQUAL(small2, 0xF);

  constexpr Unsigned<12> not_as_small = 0b101010101101;
  STATIC_REQUIRE_EQUAL(not_as_small.bit(0), 1);
  STATIC_REQUIRE_EQUAL(not_as_small.bit(1), 0);

  constexpr Unsigned<16> convert = small2;
  STATIC_REQUIRE_EQUAL(convert, 0xF);
  return passed;
}

auto test_integer() -> bool {
  bool passed = true;
  RUN_TEST(test_basic_integer());
  RUN_TEST(test_basic_unsigned());
  return passed;
}
