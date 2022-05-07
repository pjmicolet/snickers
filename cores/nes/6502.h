#pragma once
#include "../../utils/integer/integer.h"
#include "nes_ram.h"
#include "instructions.h"
#include <memory>
#include <vector>
#include <iostream>
#include <bitset>

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

using uint8 = Unsigned<8>;
using uint6 = Unsigned<6>;
using uint16 = Unsigned<16>;

using ram_ptr = std::unique_ptr<NES_RAM>;

struct StatusReg {
  StatusReg() : status_(0b00000000) {}
  auto setCarry(bool c) -> void {
    status_[0] = c;
  }
  auto setZero(bool z) -> void{
    status_[1] = z;
  }
  auto setInteruptD(bool i) -> void {
    status_[2] = i;
  }
  auto setDecimal(bool d) -> void {
    status_[3] = d;
  }
  auto setOverflow(bool o) -> void {
    status_[6] = o;
  }
  auto setNegative(bool n) -> void {
    status_[7] = n;
  }
  auto clear() -> void {
    status_ = 0b00000000;
  }

  auto isCarrySet() -> bool {
    return status_[0];
  }

  friend std::ostream& operator<<(std::ostream& os, const StatusReg& regs) {
    os << regs.status_.to_string();
    return os;
  }

  auto toString() -> std::string {
    return status_.to_string();
  }

  private:
    std::bitset<8> status_;
};

struct Registers {
  Registers() : A_(0), X_(0), Y_(0), S_(), P_(0), PC_(0) {}
  uint8 A_;
  uint8 X_;
  uint8 Y_;
  StatusReg S_;
  uint6 P_;
  uint16 PC_;
  friend std::ostream& operator<<(std::ostream& os, const Registers& regs) {
    os << "["<<regs.PC_ << "]: A[" << regs.A_ << "] X[" << regs.X_ << "] Y[" << regs.Y_ << "] S[" << regs.S_ << "] P[" << regs.P_<<"]\n";
    return os;
  }

  auto clear () -> void {
    A_ = 0; X_ = 0; Y_ = 0; S_.clear(); P_ = 0; PC_ = 0;
  }
};

enum NES_ADDRESS_MODE {
  IMPL, // implicit
  ACC,  // accumulator
  IMM,  // immediate
  ZP,   // zero page
  ZPX,  // zero page x
  ZPY,  // zero page y
  REL,  // relative
  ABS,  // absolute
  ABSX, // absolute x
  ABSY, // absolute y
  IND,  // indirect
  INDX, // indexed indirect
  INDY, // indirect indexed
};

class Instruction;

struct CPU_6502 {
  CPU_6502();

  std::unique_ptr<NES_RAM> ram_;
  auto dataFetch() -> uint8;
  auto execute() -> void;
  auto printDebug() -> void;
  auto cliOutput() -> std::string;
  auto runProgram(size_t until) -> void;
  auto clear() -> void {
    regs_.clear();
  }

protected:
  Registers regs_;

  // clang-format off
  std::array<NES_ADDRESS_MODE, 256> instToAddressMode = {
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, IMPL, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      ABS,  INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, IMPL, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, IMPL, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, IMPL, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, IMPL, IMM, IMPL, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPY, ZPY, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, IMPL, IMM, IMPL, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPY, ZPY, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, IMPL, IMM, IMPL, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, IMPL, IMM, IMPL, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX
  };

  std::array<std::string, 256> instToName = {
      "brk", "ora", "kil", "slo", "nop", "ora", "asl", "slo", "php", "ora", "asl", "anc", "nop", "ora", "asl", "slo", "bpl", "ora", "kil", "slo", "nop", "ora", "asl", "slo", "clc", "ora", "nop", "slo", "nop", "ora", "asl", "slo",
      "jsr", "and", "kil", "rla", "bit", "and", "rol", "rla", "plp", "and", "rol", "anc", "bit", "and", "rol", "rla", "bmi", "and", "kil", "rla", "nop", "and", "rol", "rla", "sec", "and", "nop", "rla", "nop", "and", "rol", "rla",
      "rti", "eor", "kil", "sre", "nop", "eor", "lsr", "sre", "pha", "eor", "lsr", "alr", "jmp", "eor", "lsr", "sre", "bvc", "eor", "kil", "sre", "nop", "eor", "lsr", "sre", "cli", "eor", "nop", "sre", "nop", "eor", "lsr", "sre",
      "rts", "adc", "kil", "rra", "nop", "adc", "ror", "rra", "pla", "adc", "ror", "arr", "jmp", "adc", "ror", "rra", "bvs", "adc", "kil", "rra", "nop", "adc", "ror", "rra", "sei", "adc", "nop", "rra", "nop", "adc", "ror", "rra",
      "nop", "sta", "nop", "sax", "sty", "sta", "stx", "sax", "dey", "nop", "txa", "xaa", "sty", "sta", "stx", "sax", "bcc", "sta", "kil", "ahx", "sty", "sta", "stx", "sax", "tya", "sta", "txs", "tas", "shy", "sta", "shx", "ahx",
      "ldy", "lda", "ldx", "lax", "ldy", "lda", "ldx", "lax", "tay", "lda", "tax", "lax", "ldy", "lda", "ldx", "lax", "bcs", "lda", "kil", "lax", "ldy", "lda", "ldx", "lax", "clv", "lda", "tsx", "las", "ldy", "lda", "ldx", "lax",
      "cpy", "cmp", "nop", "dcp", "cpy", "cmp", "dec", "dcp", "iny", "cmp", "dex", "axs", "cpy", "cmp", "dec", "dcp", "bne", "cmp", "kil", "dcp", "nop", "cmp", "dec", "dcp", "cld", "cmp", "nop", "dcp", "nop", "cmp", "dec", "dcp",
      "cpx", "sbc", "nop", "isc", "cpx", "sbc", "inc", "isc", "inx", "sbc", "nop", "sbc", "cpx", "sbc", "inc", "isc", "beq", "sbc", "kil", "isc", "nop", "sbc", "inc", "isc", "sed", "sbc", "nop", "isc", "nop", "sbc", "inc", "isc"};
  // clang-format on

private:
  auto indexFetch() -> uint16;

  inline auto resolveAddMode(uint16 PC) -> NES_ADDRESS_MODE {
    auto instruction = ram_->load(PC);
    return instToAddressMode[std::to_integer<size_t>(instruction)];
 }
 inline auto getInstructionOp() -> size_t {
   return std::to_integer<size_t>(ram_->load(regs_.PC_));
 }
 auto incrementPC() -> void;

  friend class Instruction;
  std::vector<std::unique_ptr<Instruction>> insts_;

// Just test helpers
public:
  auto getInstructionStrings() -> std::vector<std::string> {std::vector<std::string> thing{instToName.begin(),instToName.end()}; return thing; };
  auto setPC(uint16 pc ) -> void { regs_.PC_ = pc; }
  auto setX(uint8 x ) -> void { regs_.X_ = x; }
  auto setY(uint8 y ) -> void { regs_.Y_ = y; }
};
