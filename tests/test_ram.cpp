#include "../cores/snes/snes_ram.h"
#include "test_utils.h"
#include <iostream>

auto test_basic() -> bool {
  // Only a single bank, 10 lines
  bool passed = true;

  BankInfo info(10,0,{});
  BanksAndSize ram_conf = { {info}, 10 };

  SNES_RAM ram = SNES_RAM(ram_conf);
  ram.store(1, std::byte{120});

  BYTE_EQ(ram.load(1), std::byte{120});

  ram.store(2, ram.load(1));

  BYTE_EQ(ram.load(2), std::byte{120});
  BYTE_NEQ(ram.load(1), ram.load(3));
  BYTE_EQ(ram.load(1), ram.load(2));

  return passed;
}

auto test_banks() -> bool {
  bool passed = true;
  
  BanksAndSize ram_conf = { {BankInfo(100,0,{}), BankInfo(100,1,{})}, 200 };
  SNES_RAM ram = SNES_RAM(ram_conf); // That's 2 banks each with 100 lines;
                         // We run this in debug mode which will do a check
#if DEBUG
  try {
    ram.store(300, std::byte{69});
    passed &= false;
  } catch (RamException &msg) {
    std::cout << "Threw ! " << msg.what() << "\n";
  }
#endif

  ram.store(0x000019, std::byte{10}); // Bank 0 line 25
  ram.store(0x010032, std::byte{10}); // Bank 1 line 50
  ram.store(0x010019, std::byte{11}); // Bank 1 line 50

  BYTE_EQ(ram.load(0x000019), ram.load(0x010032));
  BYTE_NEQ(ram.load(0x000019), ram.load(0x010019)); // Just make sure banks are being read correctly
  REQUIRE_EQUAL(ram.addressToBank(0x010000), 1); // better way to do it

  // Only need to setup the mirror in the first one since it will set it up in the second
  BanksAndSize ram_conf_with_mirror = {
      {BankInfo(100, 0, {1}), BankInfo(100, 1, {})}, 200};
  SNES_RAM ramWithMirror = SNES_RAM(ram_conf_with_mirror);

  ramWithMirror.store(0x000001, std::byte{20});
  BYTE_EQ(ramWithMirror.load(0x000001), ramWithMirror.load(0x010001));
  BYTE_EQ(ramWithMirror.load(0x010001), std::byte{20});

  return passed;
}

auto test_ram() -> bool {
  bool passed = true;
  RUN_TEST(test_basic());
  RUN_TEST(test_banks());
  return passed;
}
