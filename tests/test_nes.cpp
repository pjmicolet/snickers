#include "../cores/nes/6502.h"
#include "../utils/assemblers/asm_utils.h"
#include "../assemblers/nes/nes.h"
#include "test_utils.h"

auto write_ram_map(CPU_6502& cpu, std::vector<uint8_t>& bytes) -> void {
  uint16_t index = 0;
  for(auto& byte : bytes) {
    cpu.ram_->store(index,std::byte{byte});
    index++;
  }
}

auto write_ram_map(CPU_6502& cpu, std::vector<std::byte>& bytes) -> void {
  uint16_t index = 0;
  for(auto& byte: bytes) {
    cpu.ram_->store(index,byte);
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
                                   0xA1,0x1A,0xB1,0x1C,0xB6,0x00,0x01,0x08,0x90,0xFF};
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
  cpu.setPC(22); // LDA with 0x1A and X being 0x2, so then we're going to load 0x1C which will then get us 0x0801
  cpu.setX(2);
  CHECK_BYTE(0x33);

  //IND,Y
  cpu.setPC(24);
  cpu.setY(2);
  CHECK_BYTE(0x0A);

  //ZP,Y
  cpu.setPC(26);
  cpu.setY(10);
  CHECK_BYTE(0x99);

  //RELATIVE, should be signed but we'll do that in the inst
  cpu.setPC(30);
  CHECK_BYTE(0xFF);
  return passed;
}

auto test_nes_basic_asm() -> bool {
  bool passed = true;
  CPU_6502 cpu{};
  NesAssembler nesAs{};

  auto ram_data = nesAs.assemble("ADC #20");
  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.execute();
  CPU_State cstate;
  cstate.A.reset(new uint8(0x20));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
    "INX\n"
    "INX\n"
    "INX\n"
    "INY\n"
  );
  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(4); // That's 3 bytes for INX INX and INY
  cstate.X.reset(new uint8(3));
  cstate.Y.reset(new uint8(1));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  // Write 0x20 to A then store it in $33 then load it in Y
  ram_data = nesAs.assemble(
    "ADC #20\n"
    "STA $33\n"
    "LDY $33\n"
  );
  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(6); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x20));
  cstate.Y.reset(new uint8(0x20));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
     "ADC #FF\n"
     "ADC #01\n"
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(4); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x00));
  cstate.P.reset(new uint8(0x03));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "ADC #12\n" //0
      "STA $45\n" //2
      "LDA #00\n" //4
      "CMP $45\n" //6
      "BEQ 04\n" //8
      "LDA #23\n" //10
      "ADC #01\n" //12
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(13); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x24));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "ADC #12\n" //0
      "STA $45\n" //2
      "LDA #12\n" //4
      "CMP $45\n" //6 this sets the carry flag as well
      "BEQ 04\n" //8
      "LDA #23\n" //10
      "ADC #01\n" //12
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(13); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x14));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "ADC #12\n" //0
      "STA $45\n" //2
      "LDA #12\n" //4
      "CMP $45\n" //6 this sets the carry flag as well
      "CLC\n" //Clear carry
      "BEQ 04\n" //8
      "LDA #23\n" //10
      "ADC #01\n" //12
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(14); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x13));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  //Baby for loop, increase A to 0x10
  ram_data = nesAs.assemble(
      "LDY #00\n" //0
      "LDX #10\n"
      "CPX #00\n"
      "INY \n"
      "DEX \n"
      "BNE FC" //jump back 4
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(10); // That's 3 bytes for INX INX and INY
  cstate.Y.reset(new uint8(0x10));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "LDA #12\n" //0
      "ORA #01\n"
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(4); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x13));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  //Basic ASL test
  ram_data = nesAs.assemble(
      "LDA #01\n" //0
      "ASL A\n"
      "CMP #02\n"
      "BEQ 03\n"
      "INX\n"
      "INY\n"
      "ASL A\n"
      "CMP #04\n"
      "BEQ 03\n"
      "INX\n"
      "INY\n"
      "ASL A\n"
      "CMP #08\n"
      "BEQ 03\n"
      "INX\n"
      "INY\n"
      "ASL A\n"
      "CMP #10\n"
      "BEQ 03\n"
      "INX\n"
      "INY\n"
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(31); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x10));
  cstate.Y.reset(new uint8(0x4));
  cstate.X.reset(new uint8(0x0));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "LDA #40\n"
      "STA $55\n"
      "LDA #04\n"
      "BIT $55\n"
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(8); // That's 3 bytes for INX INX and INY
  cstate.P.reset(new uint8(0x42));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "LDA #21\n"
      "STA $55\n"
      "LDA #21\n"
      "BIT $55\n"
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(8); // That's 3 bytes for INX INX and INY
  cstate.P.reset(new uint8(0x0));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "LDA #00\n"
      "EOR #FF\n"
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(4); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0xFF));
  cstate.P.reset(new uint8(0x80));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "LDA #01\n"
      "ROL A\n"
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(4); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x2));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "LDA #01\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"//Sets it to 0 but now carry is set
      "ROL A\n"//Now has one
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(20); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x1));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "LDA #01\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"
      "ROL A\n"//Sets it to 0 but now carry is set
      "ROR A\n"//Now has 0x80
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(20); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x80));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  ram_data = nesAs.assemble(
      "LDA #01\n"
      "PHA\n"
      "LDA #03\n"
      "PLA\n"
  );

  write_ram_map(cpu,ram_data);
  cpu.setPC(0);
  cpu.runProgram(6); // That's 3 bytes for INX INX and INY
  cstate.A.reset(new uint8(0x1));
  cstate.S.reset(new uint9(0x1FF));
  passed = cpu == cstate;
  cstate.reset();
  cpu.clear();

  return passed;
}

auto test_nes() -> bool {
  bool passed = true;
  RUN_TEST(test_nes_ram_basic());
  RUN_TEST(test_nes_data_fetch());
  RUN_TEST(test_nes_basic_asm());
  return passed;
}
