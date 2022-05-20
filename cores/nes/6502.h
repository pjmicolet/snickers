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
using uint9 = Unsigned<9>;
using uint6 = Unsigned<6>;
using uint16 = Unsigned<16>;

using ram_ptr = std::shared_ptr<NES_RAM>;

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

struct Reg {

  Reg(uint8 a) : RVal_(a), carry_(false), overflow_(false), isZero_(false), isNeg_(false) {};

  auto operator+(const uint8 otherNum) {
    uint9 res = RVal_ + (uint9)otherNum;
    bool carry = false;
    bool overflow = false;
    bool isZero = false;
    carry_ = (res&0x100 == 0x100);
    res = res & 0xFF;
    isZero_ = (res == 0);
    if((RVal_ ^ res) & (otherNum ^ res) & 0x80) {
      overflow = true;
    } else {
      overflow = false;
    }
    bool isNeg = res & 0x80;
    res = res & 0xFF;
    return Reg(res,carry,overflow,isZero, isNeg);
  }

  // It's the only way I can easily do overflow detection
  auto operator +=(const std::pair<uint8,uint8>dataCarryPair){
    uint9 tmp = RVal_ + (uint9) dataCarryPair.first + (uint9) dataCarryPair.second;
    carry_ = (tmp&0x100 == 0x100);
    tmp = tmp & 0xFF;
    isZero_ = (tmp == 0);
    if((RVal_ ^ tmp) & (dataCarryPair.first ^ tmp) & 0x80) {
      overflow_ = true;
    } else {
      overflow_ = false;
    }
    isNeg_ = tmp & 0x80;
    RVal_ = tmp;
    return this;
  }

  auto operator -=(const std::pair<uint8,uint8>dataCarryPair){
    uint9 tmp = RVal_ - (uint9) dataCarryPair.first - (1 - (uint9) dataCarryPair.second);
    carry_ = (tmp&0x100);
    tmp = tmp & 0xFF;
    isZero_ = (tmp == 0);
    if((RVal_ ^ tmp) & (dataCarryPair.first ^ tmp) & 0x80) {
      overflow_ = true;
    } else {
      overflow_ = false;
    }
    isNeg_ = tmp & 0x80;
    RVal_ = tmp;
    return this;
  }

  auto operator -=(int data){
    uint9 tmp = RVal_ - (uint9) data;
    tmp = tmp & 0xFF;
    isZero_ = (tmp == 0);
    isNeg_ = tmp & 0x80;
    RVal_ = tmp;
    return this;
  }

  auto operator &=(const uint8 num) {
    RVal_ = RVal_ & num;
    isZero_ = RVal_ == 0;
    isNeg_ = (RVal_ & 0x80);
    return this;
  }

  auto operator <<=(const uint8 num) {
    return this;
  }

  auto operator=(const uint8 num) {
    isZero_ = (num == 0);
    isNeg_ = num & 0x80;
    RVal_ = static_cast<uint9>(num);
  }

  friend std::ostream& operator<<(std::ostream& os, const Reg& regs) {
    os << regs.RVal_;
    return os;
  }

  auto toString() -> std::string {
    return std::to_string((uint16) RVal_);
  }

  operator uint8() const {
    return RVal_;
  }

  operator uint16() const {
    return static_cast<uint16>(RVal_);
  }

  // I'll eventually regret this
  auto hasCarry() -> bool {auto tmp = carry_; carry_ = false; return tmp;}
  auto isZero() -> bool {auto tmp = isZero_; isZero_ = false; return tmp;}
  auto hasOverflown() -> bool {auto tmp = overflow_; overflow_ = false; return tmp;}
  auto isNegative() -> bool {auto tmp = isNeg_; isNeg_ = false; return tmp;}

  private:
  Reg(uint9 a, bool carry, bool overflow, bool isZero, bool isNeg) : RVal_(a), carry_(carry), overflow_(overflow), isZero_(isZero), isNeg_(isNeg) {};
  private:
  uint9 RVal_;
  bool carry_;
  bool overflow_;
  bool isZero_;
  bool isNeg_;
};

struct Registers {
  Registers() : A_(0), X_(0), Y_(0), S_(0), P_(), PC_(0) {}
  Reg A_;
  Reg X_;
  Reg Y_;
  Reg S_;
  StatusReg P_;
  uint16 PC_;
  friend std::ostream& operator<<(std::ostream& os, const Registers& regs) {
    os << "["<<regs.PC_ << "]: A[" << regs.A_ << "] X[" << regs.X_ << "] Y[" << regs.Y_ << "] S[" << regs.S_ << "] P[" << regs.P_<<"]\n";
    return os;
  }

  auto clear () -> void {
    A_ = 0; X_ = 0; Y_ = 0; S_ = 0; P_.clear(); PC_ = 0;
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
  XDATA, // fetch reg
  YDATA, // fetch reg
};

enum NES_DESTINATION {
  AREG, //A Register
  ZREG, //Z Register
  YREG, //Y Register
  XREG, //Y Register
  MEM, // Memory
  PCREG, //PC Register
  SREG, // S Register
  PREG, // P Register
  NOP, // No destination
};

struct WriteBackCont {
  WriteBackCont() {}

  WriteBackCont(std::shared_ptr<NES_RAM> ram)  : ram_(ram) {}

  auto reset() -> void {
    ptr = nullptr;
    sptr = nullptr;
    pcptr = nullptr;
    memAddr_ = 0;
    carry_ = false;
    overflow_ = false;
    isZero_ = false;
    isNeg_ = false;
  }

  auto setReg(Reg& regptr) -> void { ptr = &regptr; }
  auto setReg(StatusReg& regptr) -> void { unsetReg(); sptr = &regptr; }
  auto setReg(uint16& ptr) -> void { unsetReg(); pcptr = &ptr; }
  auto unsetReg() -> void { ptr = nullptr; }
  auto setMem(uint16 memAddress) -> void { unsetReg(); memAddr_ = memAddress; }
  auto operator +=(const std::pair<uint8,uint8>dataCarryPair){
    if(ptr != nullptr) {
      *ptr += dataCarryPair;
    }
    return this;
  }

  auto operator -=(const std::pair<uint8,uint8>dataCarryPair){
    if(ptr != nullptr) {
      *ptr -= dataCarryPair;
    }
    return this;
  }

  auto operator -=(int dec){
    if(ptr != nullptr) {
      *ptr -= dec;
    }
    return this;
  }

  auto operator =(const uint8 data) {
    if(ptr != nullptr) {
      *ptr = data;
    } else {
      if(data == 0)
        isZero_ = true;
      if(data & 0x80)
        isNeg_ = true;
      ram_->store((size_t)memAddr_,std::byte{(uint8_t)data});
    }
  }

  auto operator &=(const uint8 data) {
    if(ptr != nullptr) {
      *ptr &= data;
    }
    return this;
  }

  auto operator <<=(const uint8 data) {
    if(ptr != nullptr) {
      *ptr <<= data;
    } else {
    }
    return *this;
  }

  // I'll eventually regret this
  auto hasCarry() -> bool {
    if(ptr != nullptr)
      return ptr->hasCarry();
    auto tmp = carry_; carry_ = false; return tmp;
  }
  auto isZero() -> bool {
    if(ptr != nullptr)
      return ptr->isZero();
    auto tmp = isZero_; isZero_ = false; return tmp;
  }
  auto hasOverflown() -> bool {
    if(ptr != nullptr)
      return ptr->hasOverflown();
    auto tmp = overflow_; overflow_ = false; return tmp;
  }
  auto isNegative() -> bool {
    if(ptr != nullptr)
      return ptr->isNegative();
    auto tmp = isNeg_; isNeg_ = false; return tmp;
  }

  private:
    std::shared_ptr<NES_RAM> ram_;
    uint16_t memAddr_ = 0;
    Reg* ptr = nullptr;
    StatusReg* sptr = nullptr;
    uint16* pcptr = nullptr;
    bool carry_ = false;
    bool overflow_ = false;
    bool isZero_ = false;
    bool isNeg_ = false;
};

class Instruction;

struct CPU_6502 {
  CPU_6502();

  std::shared_ptr<NES_RAM> ram_;
  auto dataFetch() -> uint8;
  auto setWriteBackCont() -> void;
  auto execute() -> void;
  auto printDebug() -> void;
  auto cliOutput() -> std::string;
  auto runProgram(size_t until) -> void;
  auto clear() -> void {
    regs_.clear();
  }

protected:
  Registers regs_;
  WriteBackCont wbc_;

  // clang-format off
  std::array<NES_ADDRESS_MODE, 256> instToAddressMode = {
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      ABS,  INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, YDATA, IMM, XDATA, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPY, ZPY,YDATA, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, ACC,  IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPY, ZPY, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, IMPL, IMM, XDATA, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX
  };

  std::array<NES_DESTINATION, 256> instToDestination = {
      SREG, AREG, NOP, NOP, NOP,  AREG, MEM, NOP, SREG, AREG, AREG, NOP, NOP, AREG, MEM, NOP, PCREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP, PREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP,
      SREG, AREG, NOP, NOP, NOP,  AREG, MEM, NOP, SREG, AREG, AREG, NOP, NOP, AREG, MEM, NOP, PCREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP, PREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP,
      PCREG,AREG, NOP, NOP, NOP,  AREG, MEM, NOP, SREG, AREG, AREG, NOP, NOP, AREG, MEM, NOP, PCREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP, PREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP,
      PCREG,AREG, NOP, NOP, NOP,  AREG, MEM, NOP, SREG, AREG, AREG, NOP, NOP, AREG, MEM, NOP, PCREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP, PREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP,
      NOP,  MEM,  MEM, MEM, MEM,  MEM, MEM, NOP,  YREG,  AREG, AREG, NOP, NOP, AREG, MEM, NOP, PCREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP, AREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP,
      YREG, AREG, XREG,XREG,YREG, AREG, XREG,XREG,YREG,AREG, XREG, NOP, NOP, AREG, MEM, NOP, PCREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP, PREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP,
      PREG, AREG, NOP, NOP, NOP,  AREG, MEM, NOP, YREG, AREG, XREG, NOP, NOP, AREG, MEM, NOP, PCREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP, PREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP,
      PREG, AREG, NOP, NOP, NOP,  AREG, MEM, NOP, XREG, AREG, AREG, NOP, NOP, AREG, MEM, NOP, PCREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP, PREG, AREG, NOP, NOP, NOP, AREG, MEM, NOP,
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

  inline auto resolveDestMode(uint16 PC) -> NES_DESTINATION {
    auto instruction = ram_->load(PC);
    return instToDestination[std::to_integer<size_t>(instruction)];
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
