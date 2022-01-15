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

#define CHECK_BYTE(X) REQUIRE_EQUAL(cpu.dataFetch(),X);

auto test_nes_data_fetch() -> bool {
  bool passed = true;
  CPU_6502 cpu{};
  //We have an LDA with #0x33 immediate
  //LDA ZP that fetches from $10 which has 0x99
  //LDA X with base 0x1
  std::vector<uint8_t> ram_data = {0xA9,0x33,0xA5,0x0A,0x12,0x00,0x39,0x44,0x00,0x00,0x99,
                                   0xB5,0x5,0xAD,0x07,0x00,0xBD,0x01,0x00,0xB9,0x00,0x08,
                                   0xA1,0x05,0xB1,0x12};
  write_ram_map(cpu,ram_data);

  //IMMEDIATE
  cpu.setPC(0); // LDA #0x33
  CHECK_BYTE(0x33);

  //ZP
  cpu.setPC(2); // LDA ZP (0x0A)
  CHECK_BYTE(0x99);

  //ZPX
  cpu.setPC(11);
  cpu.setX(1);
  CHECK_BYTE(0x39) // LDA ZP,X (0x5)

  //ABS
  cpu.setPC(13);
  CHECK_BYTE(0x44) // LDA 0x0007

  //ABSX
  cpu.setPC(16);
  cpu.setX(3);
  CHECK_BYTE(0x12); // LDA ABS,X (0x0001)
  cpu.setX(0);

  //ABSY
  cpu.setPC(19);
  cpu.setY(1);
  CHECK_BYTE(0x33); // LDA ABS,Y (0x0800) this should also test the memory banks

  //IND,X
  cpu.setPC(22); // So (0x05+2+1) 0x08 | 0x07 (0x05+2)
  cpu.setX(2);
  CHECK_BYTE(0x44);

  //IND,Y
  return passed;
}

auto test_nes() -> bool {
  bool passed = true;
  RUN_TEST(test_nes_ram_basic());
  RUN_TEST(test_nes_data_fetch());
  return passed;
}
