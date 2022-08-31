#pragma once
#include "../../utils/integer/integer.h"
#include <bitset>

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

using uint8 = Unsigned<8>;
using uint9 = Unsigned<9>;
using uint6 = Unsigned<6>;
using uint16 = Unsigned<16>;

// Status register represents the P register in the 6502
// These are just utilities to make it much clearer to use :)
struct StatusReg {
  StatusReg();
  auto setCarry(bool c) -> void;
  auto setZero(bool z) -> void;
  auto setInteruptD(bool i) -> void;
  auto setDecimal(bool d) -> void;
  auto setOverflow(bool o) -> void;
  auto setNegative(bool n) -> void;
  auto clear() -> void;
  auto operator=(uint8 pset) -> StatusReg&;
  auto isCarrySet() -> bool;
  auto isZeroSet() -> bool;
  auto isNegSet() -> bool;
  auto isOverflowSet() -> bool;

  friend std::ostream& operator<<(std::ostream& os, const StatusReg& regs);
  auto toString() -> std::string;

  //Absolute abomination
  auto toUint8() -> uint8;

  private:
    std::bitset<8> status_;
};

// Reg represents any of the other registers within the 6502
// It handles setting flags within the P register and also does all the overflow checking
struct Reg {

  Reg(uint8 a);

  auto operator+(const uint8 otherNum) -> Reg;
  auto operator +=(const std::pair<uint8,uint8>dataCarryPair) -> Reg&;
  auto operator -=(const std::pair<uint8,uint8>dataCarryPair) -> Reg&;
  auto operator -=(int data) -> Reg&;
  auto operator &=(const uint8 num) -> Reg&;
  auto operator |=(const uint8 num) -> Reg&;
  auto operator <<=(const uint8 num) -> Reg&;
  auto operator >>=(const uint8 num) -> Reg&;
  auto operator ^=(const uint8 num) -> Reg&;
  auto operator=(const uint8 num) -> Reg&;
  auto rol(const bool carry) -> Reg&;
  auto ror(const bool carry) -> Reg&;

  friend std::ostream& operator<<(std::ostream& os, const Reg& regs) {
    os << regs.RVal_;
    return os;
  }

  auto toString() -> std::string;

  operator uint8() const {
    return static_cast<uint8>(RVal_);
  }

  operator size_t() const {
    return static_cast<size_t>(static_cast<uint8>(RVal_));
  }

  operator uint16() const {
    return static_cast<uint16>(RVal_);
  }

  // I'll eventually regret this
  auto hasCarry() -> bool;
  auto isZero() -> bool;
  auto hasOverflown() -> bool;
  auto isNegative() -> bool;
  auto keepCarry() -> void;

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
  Registers() : A_(0), X_(0), Y_(0), S_(0x1FD), P_(), PC_(0) {
    P_ = 36;
  }
  Reg A_;
  Reg X_;
  Reg Y_;
  uint9 S_;
  StatusReg P_;
  uint16 PC_;
  friend std::ostream& operator<<(std::ostream& os, const Registers& regs) {
    os << "["<< std::hex << regs.PC_ << "]: A[" << regs.A_ << "] X[" << regs.X_ << "] Y[" << regs.Y_ << "] S[" << regs.S_ << "] P[" << regs.P_<<"]\n";
    return os;
  }

  auto clear () -> void {
    A_ = 0; X_ = 0; Y_ = 0; S_ = 0x1FD; P_.clear(); P_ = 36; PC_ = 0;
  }
};
