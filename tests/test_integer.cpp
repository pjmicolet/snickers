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
  return true;
}

auto test_integer() -> bool {
  bool passed = true;
  RUN_TEST(test_basic_integer());
  return passed;
}
