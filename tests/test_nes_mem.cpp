#include "../cores/nes/6502.h"
#include "test_utils.h"

auto test_nes_ram_basic() -> bool {
  bool passed = true;
  CPU_6502 cpu{};  
  cpu.ram_->store(0x0, std::byte{12});
  BYTE_EQ(cpu.ram_->load(0x1000), std::byte{12});
  BYTE_EQ(cpu.ram_->load(0x0800), std::byte{12});
  cpu.ram_->store(0x1001, std::byte{244});
  BYTE_EQ(cpu.ram_->load(0x1), std::byte{244});
  return passed;
}

auto test_nes_ram() -> bool {
  bool passed = true;
  RUN_TEST(test_nes_ram_basic());
  return passed;
}
