#include "../core/rom.h"
#include "test_utils.h"
#include <iostream>

auto test_basic_load() -> bool {
  try {
    Rom rom("./Ladida_lol.sfc");
    if (rom.size() == 0)
      return false;
  } catch (RomException &e) {
    std::cout << __func__ << " failed to load rom\n";
    return false;
  }

  return true;
}

auto test_wrong_path() -> bool {
  bool pass = false;
  try {
    Rom rom("nowhereland");
  } catch (RomException &e) {
    pass = true;
  }

  return pass;
}

auto test_rom() -> bool {
  bool passed = true;
  RUN_TEST(test_basic_load());
  RUN_TEST(test_wrong_path());
  return passed;
}
