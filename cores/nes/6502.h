#pragma once
#include "../../utils/integer/integer.h"
#include "nes_ram.h"
#include "instructions.h"
#include "registers.h"
#include <memory>
#include <vector>
#include <array>
#include <iostream>

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

using uint8 = Unsigned<8>;
using uint9 = Unsigned<9>;
using uint6 = Unsigned<6>;
using uint16 = Unsigned<16>;

using ram_ptr = std::shared_ptr<NES_RAM>;


enum NES_ADDRESS_MODE {
  IMPL, // implicit
  ACC,  // accumulator
  IMM,  // immediate
  ZP,   // zero page
  ZPX,  // zero page x
  ZPY,  // zero page y
  REL,  // relative
  ABS,  // absolute
  ABSADDR,  // absolute address for jmp
  ABSX, // absolute x
  ABSY, // absolute y
  IND,  // indirect
  INDX, // indexed indirect
  INDY, // indirect indexed
  XDATA, // fetch reg
  YDATA, // fetch reg
  SDATA, // fetch data from SReg
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
    stptr = nullptr;
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
  auto unsetReg() -> void { ptr = nullptr; sptr = nullptr; stptr = nullptr; }
  auto setMem(uint16 memAddress) -> void { unsetReg(); memAddr_ = memAddress; }
  auto setStack(uint9& ptrAddr) -> void { unsetReg(); stptr = &ptrAddr; }
  auto operator +=(const std::pair<uint8,uint8>dataCarryPair){
    if(ptr != nullptr) {
      *ptr += dataCarryPair;
    } else {
      std::cout << "Shouldnt be here\n";
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
    if( stptr != nullptr ) {
      ram_->store(static_cast<size_t>(*stptr),std::byte{(uint8_t)data});
      *stptr-= 1;
    } else if(ptr != nullptr) {
        *ptr = data;
    } else if(sptr != nullptr){
        *sptr = data;
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

  auto operator |=(const uint8 data) {
    if(ptr != nullptr) {
      *ptr |= data;
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

  auto operator >>=(const uint8 data) {
    if(ptr != nullptr) {
      *ptr >>= data;
    } else {
      auto init = mload();
      carry_ = init & 0x1;
      init >>= 1;
      isZero_ = init == 0;
      isNeg_ = (init & 0x80);
      mstore(init);
    }
    return *this;
  }

  auto operator ^=(const uint8 data) {
    if(ptr != nullptr) {
      *ptr ^= data;
    } else {
      std::cout << "Shouldn't be here\n";
    }
    return *this;
  }

  auto rol(const bool carry) {
    if(ptr!= nullptr) {
      ptr->rol(carry);
    } else {
      auto init = mload();
      carry_ = init & 0x80;
      init <<= 1;
      init &= 0xFF;
      init |= carry;
      isZero_ = (init == 0);
      isNeg_ = init & 0x80;
      mstore(init);
    }
    return *this;
  }

  auto ror(const bool carry) {
    if(ptr!= nullptr) {
      ptr->ror(carry);
    } else {
      auto init = mload();
      carry_ = init & 0x1;
      init >>= 1;
      init &= 0xFF;
      if(carry)
        init |= 0x80;
      isZero_ = (init == 0);
      isNeg_ = init & 0x80;
      mstore(init);
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
    uint9* stptr = nullptr;
    StatusReg* sptr = nullptr;
    uint16* pcptr = nullptr;
    bool carry_ = false;
    bool overflow_ = false;
    bool isZero_ = false;
    bool isNeg_ = false;
    bool isStack_ = false;
  private:
    auto mload() -> uint9 {
      return static_cast<uint9>(std::to_integer<uint8_t>(ram_->load(static_cast<size_t>(memAddr_))));
    };
    auto mstore(uint9 data) -> void {
      ram_->store(static_cast<size_t>(memAddr_),std::byte{static_cast<uint8_t>(data)});
    };
};

class Instruction;

struct CPU_State {
  CPU_State() {
    A = std::unique_ptr<uint8>();
    X = std::unique_ptr<uint8>();
    Y = std::unique_ptr<uint8>();
    S = std::unique_ptr<uint9>();
    P = std::unique_ptr<uint8>();
    PC = std::unique_ptr<uint16>();
  }

  auto reset() -> void {
    A.reset(nullptr);
    X.reset(nullptr);
    Y.reset(nullptr);
    S.reset(nullptr);
    P.reset(nullptr);
    PC.reset(nullptr);
  }
  std::unique_ptr<uint8> A;
  std::unique_ptr<uint8> X;
  std::unique_ptr<uint8> Y;
  std::unique_ptr<uint9> S;
  std::unique_ptr<uint8> P;
  std::unique_ptr<uint16> PC;
};

struct CPU_6502 {
  CPU_6502();

  std::shared_ptr<NES_RAM> ram_;
  auto dataFetch() -> uint16;
  auto setWriteBackCont() -> void;
  auto execute() -> void;
  auto printDebug() -> void;
  auto cliOutput() -> std::string;
  auto runProgram(size_t until) -> void;
  auto debugRun(size_t until, std::vector<std::vector<uint16_t>>& data) -> bool;
  auto clear() -> void {
    regs_.clear();
    ram_->clear();
  }

  auto operator==(const CPU_State& cstate) -> bool {
    bool theSame = true;
    if(cstate.A.get()) {
      theSame &= (*(cstate.A) == (uint8)regs_.A_);
      if(!theSame)
        std::cout << "Mismatch A " << *(cstate.A) << " " << regs_.A_ << "\n";
    }
    if(cstate.X.get()) {
      theSame &= (*(cstate.X) == (uint8)regs_.X_);
      if(!theSame)
        std::cout << "Mismatch X " << *(cstate.X) << " " << regs_.X_ << "\n";
    }
    if(cstate.Y.get()) {
      theSame &= (*(cstate.Y) == (uint8)regs_.Y_);
      if(!theSame)
        std::cout << "Mismatch Y " << *(cstate.Y) << " " << regs_.Y_ << "\n";
    }
    if(cstate.S.get()) {
      theSame &= (*(cstate.S) == (uint9)regs_.S_);
      if(!theSame)
        std::cout << "Mismatch S " << *(cstate.S) << " " << regs_.S_ << "\n";
    }
    if(cstate.P.get()) {
      theSame &= (*(cstate.P) == regs_.P_.toUint8());
      if(!theSame)
        std::cout << "Mismatch P " << *(cstate.P) << " " << regs_.P_.toUint8() << "\n";
    }
    if(cstate.PC.get()) {
      theSame &= (*(cstate.PC) == regs_.PC_);
      if(!theSame)
        std::cout << "Mismatch PC " << *(cstate.PC) << " " << regs_.PC_ << "\n";
    }
    return theSame;
  }

protected:
  Registers regs_;
  WriteBackCont wbc_;
  bool branchTaken_;

  // clang-format off
  std::array<NES_ADDRESS_MODE, 256> instToAddressMode = {
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      ABSADDR,  INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, ACC, IMM, ACC, IMM, ABSADDR, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMPL, INDX, IMPL, INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, IND, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, YDATA, IMM, XDATA, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPY, ZPY,YDATA, ABSY, XDATA, ABSY, ABSX, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, ACC,  IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPY, ZPY, IMPL, ABSY, SDATA, ABSY, ABSX, ABSX, ABSY, ABSY,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, IMPL, IMM, XDATA, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSY, ABSX, ABSX, ABSX,
      IMM,  INDX, IMM,  INDX, ZP, ZP, ZP, ZP, IMPL, IMM, ACC, IMM, ABS, ABS, ABS, ABS, REL, INDY, IMPL, INDY, ZPX, ZPX, ZPX, ZPX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX
  };

  std::array<NES_DESTINATION, 256> instToDestination = {
      SREG, AREG, NOP, MEM, NOP,  AREG, MEM, MEM, SREG, AREG, AREG, NOP, NOP, AREG, MEM, MEM, PCREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM, PREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM,
      SREG, AREG, NOP, MEM, NOP,  AREG, MEM, MEM, PREG, AREG, AREG, NOP, NOP, AREG, MEM, MEM, PCREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM, PREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM,
      PCREG,AREG, NOP, MEM, NOP,  AREG, MEM, MEM, SREG, AREG, AREG, NOP, NOP, AREG, MEM, MEM, PCREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM, PREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM,
      PCREG,AREG, NOP, MEM, NOP,  AREG, MEM, MEM, AREG, AREG, AREG, NOP, NOP, AREG, MEM, MEM, PCREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM, PREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM,
      NOP,  MEM,  MEM, MEM, MEM,  MEM, MEM,  MEM,  YREG,  AREG, AREG, NOP, MEM, MEM, MEM, MEM, PCREG, MEM, NOP, NOP, MEM, MEM, MEM,  MEM, AREG,  MEM, SREG, NOP, NOP, MEM, MEM, NOP,
      YREG, AREG, XREG,XREG,YREG, AREG, XREG,XREG,YREG,AREG, XREG, NOP,  YREG, AREG, XREG, NOP, PCREG, AREG, NOP, NOP, YREG, AREG, XREG, NOP, PREG, AREG, XREG, NOP, YREG, AREG, XREG, NOP,
      PREG, AREG, NOP, MEM, NOP,  AREG, MEM, MEM, YREG, AREG, XREG, NOP, NOP, AREG, MEM, MEM, PCREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM, PREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM,
      PREG, AREG, NOP, MEM, NOP,  AREG, MEM, MEM, XREG, AREG, AREG, AREG, NOP, AREG, MEM, MEM, PCREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM, PREG, AREG, NOP, MEM, NOP, AREG, MEM, MEM,
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
protected:
  auto popStack() -> uint8 {
    regs_.S_ += 1;
    return static_cast<uint8>(std::to_integer<uint8_t>(ram_->load(static_cast<size_t>(regs_.S_))));
  }

  auto pushStack(uint16 data) -> void {
    uint8 highByte = (data & 0xFF00) >> 8;
    uint8 lowByte = (data & 0xFF);
    pushStack(highByte);
    pushStack(lowByte);
  }

  auto popPCFromStack() -> uint16 {
    uint8 lowByte = popStack();
    uint8 highByte = popStack();
    uint16 pc = highByte;
    pc <<= 8;
    pc |= lowByte;
    return pc;
  }

  inline auto pushStack(uint8 data) -> void {
    ram_->store(static_cast<size_t>(regs_.S_),std::byte{(uint8_t)data});
    regs_.S_ -= 1;
  }
};
