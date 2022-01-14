#include "../cores/nes/6502.h"
#include "test_utils.h"

auto write_ram_map(CPU_6502& cpu, std::vector<uint8_t>& bytes) -> void {
  uint16_t index = 0;
  for(auto& byte : bytes) {
    cpu.ram_->store(index,std::byte{byte});
    index++;
  }
}

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
  //We have an LDA with #0x33 immediate
  //LDA ZP that fetches from $10 which has 0x99
  //LDA X with base 0x1
  std::vector<uint8_t> ram_data = {0xA9,0x33,0xA5,0x0A,0x00,0x00,0x39,0x00,0x00,0x00,0x99,0xB5,0x5};
  write_ram_map(cpu,ram_data);

  cpu.setPC(0);
  auto data = cpu.dataFetch();
  REQUIRE_EQUAL(data, 0x33);

  cpu.setPC(2);
  data = cpu.dataFetch();
  REQUIRE_EQUAL(data, 0x99);

  cpu.setPC(11);
  cpu.setX(1);
  data = cpu.dataFetch();
  REQUIRE_EQUAL(data, 0x39)
  return passed;
}

auto test_nes() -> bool {
  bool passed = true;
  RUN_TEST(test_nes_ram_basic());
  RUN_TEST(test_nes_data_fetch());
  return passed;
}
