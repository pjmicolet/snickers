#pragma once
#include "../../utils/assemblers/asm_utils.h"
#include <utility>

#define MATCH_INST(instname,num) \
  if (match(instname,inst)) \
    return {start,start+(num-1)};\
  start+= num;

#define IMPL(hex) std::make_pair(implicit,hex),
#define ACC(hex) std::make_pair(accumulator,hex),
#define IMM(hex) std::make_pair(immediate,hex),
#define ZP(hex) std::make_pair(zeroPage,hex),
#define ZPX(hex) std::make_pair(zeroPageX,hex),
#define ZPY(hex) std::make_pair(zeroPageY,hex),
#define REL(hex) std::make_pair(relative,hex),
#define ABS(hex) std::make_pair(absolute,hex),
#define ABSX(hex) std::make_pair(absoluteX,hex),
#define ABSY(hex) std::make_pair(absoluteY,hex),
#define IND(hex) std::make_pair(indirect,hex),
#define INDX(hex) std::make_pair(indirectX,hex),
#define INDY(hex) std::make_pair(indirectY,hex),

using opcode = std::pair<const std::string_view, uint8_t>;
using offsets = std::array<const size_t,2>;

struct OpTable {
  constexpr OpTable() {}
  constexpr auto getOpcode(const std::string_view& inst) const -> uint8_t {
    const auto instTypeSplit = strSplit<2>(inst);
    const auto delim = getOffsets(instTypeSplit[0]);
    for(size_t i = delim[0]; i <= delim[1]; i++) {
      if (match(opcodes[i].first,instTypeSplit[1]))
        return opcodes[i].second;
    }
    return 0;
  }
  private:
    static inline constexpr std::string_view implicit = "";
    static inline constexpr std::string_view accumulator = "A";
    static inline constexpr std::string_view immediate = "#@digits";
    static inline constexpr std::string_view zeroPage = "$@digits";
    static inline constexpr std::string_view zeroPageX = "$@digits,X";
    static inline constexpr std::string_view zeroPageY = "$@digits,Y";
    static inline constexpr std::string_view relative = "@name";
    static inline constexpr std::string_view absolute = "$@digits";
    static inline constexpr std::string_view absoluteX = "$@digits,X";
    static inline constexpr std::string_view absoluteY = "$@digits,Y";
    static inline constexpr std::string_view indirect = "($@digits)";
    static inline constexpr std::string_view indirectX = "($@digits,X)";
    static inline constexpr std::string_view indirectY = "($@digits),Y";
    const std::array<opcode, 151> opcodes = {
      //ADC
      IMM(0x69) ZP(0x65) ZPX(0x75) ABS(0x6D) ABSX(0x7D) ABSY(0x79) INDX(0x61) INDY(0x71)
      //AND
      IMM(0x29) ZP(0x25) ZPX(0x35) ABS(0x2D) ABSX(0x3D) ABSY(0x39) INDX(0x21) INDY(0x31)
      //ASL
      ACC(0x0A) ZP(0x06) ZPX(0x16) ABS(0x0E) ABSX(0x1E)
      REL(0x90) //BCC
      REL(0xB0) //BCS
      REL(0xF0) //BEQ
      //BIT
      ZP(0x24) ABS(0x2C)
      REL(0x30) //BMI
      REL(0xD0) //BNE
      REL(0x10) //BPL
      IMPL(0x00) //BRK
      REL(0x50) //BVC
      REL(0x70) //BVS
      IMPL(0x18) //CLC
      IMPL(0xD8)//CLD
      IMPL(0x58)//CLI
      IMPL(0xB8)//CLV
      //CMP
      IMM(0xC9) ZP(0xC5) ZPX(0xD5) ABS(0xCD) ABSX(0xDD) ABSY(0xD9) INDX(0xC1) INDY(0xD1)
      //CPX
      IMM(0xE0) ZP(0xE4) ABS(0xEC)
      //CPY
      IMM(0xC0) ZP(0xC4) ABS(0xCC)
      //DEC
      ZP(0xC6) ZPX(0xD6) ABS(0xCE) ABSX(0xDE)
      IMPL(0xCA) //DEX
      IMPL(0x88)//DEY
      //EOR
      IMM(0x49) ZP(0x45) ZPX(0x55) ABS(0x4D) ABSX(0x5D) ABSY(0x59) INDX(0x41) INDY(0x51)
      //INC
      ZP(0xE6) ZPX(0xF6) ABS(0xEE) ABSX(0xFE)
      IMPL(0xE8)//INX
      IMPL(0xC8)//INY
      //JMP
      ABS(0x4C) IND(0x6C)
      ABS(0x20)//JSR
      //LDA
      IMM(0xA9) ZP(0xA5) ZPX(0xB5) ABS(0xAD) ABSX(0xBD) ABSY(0xB9) INDX(0xA1) INDY(0xB1)
      //LDX
      IMM(0xA2) ZP(0xA6) ZPY(0xB6) ABS(0xAE) ABSY(0xBE)
      //LDY
      IMM(0xA0) ZP(0xA4) ZPX(0xB4) ABS(0xAC) ABSX(0xBC)
      //LSR
      ACC(0x4A) ZP(0x46) ZPX(0x56) ABS(0x4E) ABSX(0x5E)
      IMPL(0xEA)//NOP
      //ORA
      IMM(0x09) ZP(0x05) ZPX(0x15) ABS(0x0D) ABSX(0x1D) ABSY(0x19) INDX(0x01) INDY(0x11)
      IMPL(0x48)//PHA
      IMPL(0x08)//PHP
      IMPL(0x68)//PLA
      IMPL(0x28)//PLP
      //ROL
      ACC(0x2A) ZP(0x26) ZPX(0x36) ABS(0x2E) ABSX(0x3E)
      //ROR
      ACC(0x6A) ZP(0x66) ZPX(0x76) ABS(0x6E) ABSX(0x7E)
      IMPL(0x40)//RTI
      IMPL(0x60)//RTS
      //SBC
      IMM(0xE9) ZP(0xE5) ZPX(0xF5) ABS(0xED) ABSX(0xFD) ABSY(0xF9) INDX(0xE1) INDY(0xF1)
      IMPL(0x38)//SEC
      IMPL(0xF8)//SED
      IMPL(0x78)//SEI
      //STA
      ZP(0x85) ZPX(0x95) ABS(0x8D) ABSX(0x9D) ABSY(0x99) INDX(0x81) INDY(0x91)
      //STX
      ZP(0x86) ZPY(0x96) ABS(0x8E)
      //STY
      ZP(0x84) ZPX(0x94) ABS(0x8C)
      IMPL(0xAA)//TAX
      IMPL(0xA8)//TAY
      IMPL(0xBA)//TSX
      IMPL(0x8A)//TXA
      IMPL(0x9A)//TXS
      IMPL(0x98)//TYA
    };
  public:
    constexpr auto getOffsets(const std::string_view& inst) const -> offsets {
      size_t start = 0;
      MATCH_INST("ADC",8) MATCH_INST("AND",8) MATCH_INST("ASL",5) MATCH_INST("BCC",1) MATCH_INST("BCS",1)
      MATCH_INST("BEQ",1) MATCH_INST("BIT",2) MATCH_INST("BMI",1) MATCH_INST("BNE",1) MATCH_INST("BPL",1)
      MATCH_INST("BRK",1) MATCH_INST("BVC",1) MATCH_INST("BVS",1) MATCH_INST("CLC",1) MATCH_INST("CLD",1)
      MATCH_INST("CLI",1) MATCH_INST("CLV",1) MATCH_INST("CMP",8) MATCH_INST("CPX",3) MATCH_INST("CPY",3)
      MATCH_INST("DEC",4) MATCH_INST("DEX",1) MATCH_INST("DEY",1) MATCH_INST("EOR",8) MATCH_INST("INC",4)
      MATCH_INST("INX",1) MATCH_INST("INY",1) MATCH_INST("JMP",2) MATCH_INST("JSR",1) MATCH_INST("LDA",8)
      MATCH_INST("LDX",5) MATCH_INST("LDY",5) MATCH_INST("LSR",5) MATCH_INST("NOP",1) MATCH_INST("ORA",8)
      MATCH_INST("PHA",1) MATCH_INST("PHP",1) MATCH_INST("PLA",1) MATCH_INST("PLP",1) MATCH_INST("ROL",5)
      MATCH_INST("ROR",5) MATCH_INST("RTI",1) MATCH_INST("RTS",1) MATCH_INST("SBC",8) MATCH_INST("SEC",1)
      MATCH_INST("SED",1) MATCH_INST("SEI",1) MATCH_INST("STA",7) MATCH_INST("STX",3) MATCH_INST("STY",3)
      MATCH_INST("TAX",1) MATCH_INST("TAY",1) MATCH_INST("TSX",1) MATCH_INST("TXA",1) MATCH_INST("TXS",1)
      MATCH_INST("TYA",1)
      return {0,0};
    }
};

constexpr auto getTable() -> OpTable{
  constexpr OpTable a{};
  return a;
}
struct NesAssembler {
  public:
  constexpr NesAssembler() {};
  constexpr auto getOffsets(const std::string_view inst) const -> offsets {
    return table_.getOffsets(inst);
  }
  constexpr auto getOpcode(const std::string_view inst) -> uint8_t {
    return table_.getOpcode(inst);
  }
  private:
  //Store some table with the opcodes and their pattern matches, then we just scan the right
  //instruction, go through the potential opcode matches and return the opcode
  static constexpr OpTable table_{};
};

#undef IMPL
#undef ACC
#undef IMM
#undef ZP
#undef ZPX
#undef ZPY
#undef REL
#undef ABS
#undef ABSX
#undef ABSY
#undef IND
#undef INDX
#undef INDY
