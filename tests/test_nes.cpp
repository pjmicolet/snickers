#include "../cores/nes/6502.h"
#include "test_utils.h"

auto test_nes_ram_basic() -> bool {
  bool passed = true;
  CPU_6502 cpu{};  
  cpu.ram_->store(0x0, std::byte{12});
  BYTE_EQ(cpu.ram_->load(0x1000), std::byte{12});
  BYTE_NEQ(cpu.ram_->load(0x8), std::byte{12});
  BYTE_EQ(cpu.ram_->load(0x0800), std::byte{12});
  cpu.ram_->store(0x1001, std::byte{244});
  BYTE_EQ(cpu.ram_->load(0x1), std::byte{244});
  return passed;
}

auto test_nes_data_fetch() -> bool {
  bool passed = true;
  CPU_6502 cpu{};
  cpu.ram_->store(0x0, std::byte{0xA9}); // This is an LDA with #0x33 immediate
  cpu.ram_->store(0x1, std::byte{0x33}); // This is the #0x33

  cpu.ram_->store(0x2, std::byte{0xA5}); // This is an LDA ZP
  cpu.ram_->store(0x3, std::byte{0x10}); // Going to fetch from $10
  cpu.ram_->store(0x10, std::byte{0x99}); // Put 0x99 in $10

  cpu.setPC(0);
  auto data = cpu.dataFetch();
  REQUIRE_EQUAL(data, 0x33);

  cpu.setPC(2);
  data = cpu.dataFetch();
  REQUIRE_EQUAL(data, 0x99);
  return passed;
}

auto test_nes() -> bool {
  bool passed = true;
  RUN_TEST(test_nes_ram_basic());
  RUN_TEST(test_nes_data_fetch());
  return passed;
}
