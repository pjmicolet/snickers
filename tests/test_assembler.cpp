#include "../utils/assemblers/asm_utils.h"
#include <iostream>
#include "test_utils.h"

auto test_asm_utils() -> bool {
  bool passed = true;
  static_assert(strSplit<4>("a b cd ef").size() == 4);
  constexpr auto result = strSplit<5>("hey how are you today");
  static_assert(result[0] == "hey");
  static_assert(result[1] == "how");
  static_assert(result[2] == "are");
  static_assert(result[3] == "you");
  static_assert(result[4] == "today");
  return passed;
}

auto test_asm() -> bool {
  bool passed = true;
  RUN_TEST(test_asm_utils());
  return passed;
}
