#include "registers.h"

StatusReg::StatusReg() : status_(0x24) {}

auto StatusReg::setCarry(bool c) -> void {
  status_[0] = c;
}

auto StatusReg::setZero(bool z) -> void{
  status_[1] = z;
}
auto StatusReg::setInteruptD(bool i) -> void {
  status_[2] = i;
}
auto StatusReg::setDecimal(bool d) -> void {
  status_[3] = d;
}
auto StatusReg::setOverflow(bool o) -> void {
  status_[6] = o;
}
auto StatusReg::setNegative(bool n) -> void {
  status_[7] = n;
}
auto StatusReg::clear() -> void {
  status_ = 0x24;
}

auto StatusReg::operator=(uint8 pset) -> StatusReg& {
  status_[0] = pset & 0x1;
  status_[1] = pset & 0x2;
  status_[2] = pset & 0x4;
  status_[3] = pset & 0x8;
  status_[6] = pset & 0x40;
  status_[7] = pset & 0x80;
  return *this;
}

auto StatusReg::isCarrySet() -> bool {
  return status_[0];
}

auto StatusReg::isZeroSet() -> bool {
  return status_[1];
}
auto StatusReg::isNegSet() -> bool {
  return status_[7];
}
auto StatusReg::isOverflowSet() -> bool {
  return status_[6];
}

auto operator<<(std::ostream& os, const StatusReg& regs) -> std::ostream&  {
  os << regs.status_.to_string();
  return os;
}

auto StatusReg::toString() -> std::string {
  return status_.to_string();
}

//Absolute abomination
auto StatusReg::toUint8() -> uint8 {
  return static_cast<uint8>(static_cast<int>(status_.to_ulong()));
}


Reg::Reg(uint8 a) : RVal_(a), carry_(false), overflow_(false), isZero_(false), isNeg_(false) {};

auto Reg::operator+(const uint8 otherNum) -> Reg {
  uint9 res = RVal_ + (uint9)otherNum;
  bool carry = false;
  bool overflow = false;
  bool isZero = false;
  carry_ = ((res&0x100) == 0x100);
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
auto Reg::operator +=(const std::pair<uint8,uint8>dataCarryPair) -> Reg& {
  uint9 tmp = RVal_ + (uint9) dataCarryPair.first + (uint9) dataCarryPair.second;
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
  return *this;
}

auto Reg::operator -=(const std::pair<uint8,uint8>dataCarryPair) -> Reg& {
  uint9 tmp = RVal_ - (uint9) dataCarryPair.first - (1 - (uint9) dataCarryPair.second);
  carry_ = !(tmp&0x100);
  tmp = tmp & 0xFF;
  isZero_ = (tmp == 0);
  if((((uint8)RVal_ ^ dataCarryPair.first) & 0x80) & ((uint8)RVal_ ^ tmp) & 0x80) {
    overflow_ = true;
  } else {
    overflow_ = false;
  }
  isNeg_ = tmp & 0x80;
  RVal_ = tmp;
  return *this;
}

auto Reg::operator -=(int data) -> Reg&{
  uint9 tmp = RVal_ - (uint9) data;
  tmp = tmp & 0xFF;
  isZero_ = (tmp == 0);
  isNeg_ = tmp & 0x80;
  RVal_ = tmp;
  return *this;
}

auto Reg::operator &=(const uint8 num) -> Reg& {
  RVal_ = RVal_ & num;
  isZero_ = RVal_ == 0;
  isNeg_ = (RVal_ & 0x80);
  return *this;
}

auto Reg::operator |=(const uint8 num) -> Reg& {
  RVal_ = RVal_ | num;
  isZero_ = RVal_ == 0;
  isNeg_ = (RVal_ & 0x80);
  return *this;
}

auto Reg::operator <<=(const uint8 num) -> Reg& {
  carry_ = RVal_ & 0x80;
  RVal_ <<= 1;
  RVal_ = RVal_ & 0xFF;
  isZero_ = RVal_ == 0;
  isNeg_ = RVal_ & 0x80;
  return *this;
}

auto Reg::operator >>=(const uint8 num) -> Reg& {
  carry_ = RVal_ & 0x1;
  RVal_ >>= 1;
  RVal_ = RVal_ & 0xFF;
  isZero_ = RVal_ == 0;
  isNeg_ = RVal_ & 0x80;
  return *this;
}

auto Reg::operator ^=(const uint8 num) -> Reg& {
  RVal_ = (RVal_ ^ num) & 0xFF;
  isZero_ = RVal_ == 0;
  isNeg_ = RVal_ & 0x80;
  return *this;
}

auto Reg::operator=(const uint8 num) -> Reg& {
  isZero_ = (num == 0);
  isNeg_ = num & 0x80;
  RVal_ = static_cast<uint9>(num);
  return *this;
}

auto Reg::rol(const bool carry) -> Reg& {
  carry_ = RVal_ & 0x80;
  RVal_ <<= 1;
  RVal_ &= 0xFF;
  RVal_ |= carry;
  isZero_ = (RVal_ == 0);
  isNeg_ = RVal_ & 0x80;
  return *this;
}

auto Reg::ror(const bool carry) -> Reg&{
  carry_ = RVal_ & 0x1;
  RVal_ >>= 1;
  RVal_ &= 0xFF;
  if(carry)
    RVal_ |= 0x80;
  isZero_ = (RVal_ == 0);
  isNeg_ = RVal_ & 0x80;
  return *this;
}

auto Reg::toString() -> std::string {
  return std::to_string((uint16) RVal_);
}

// I'll eventually regret this
auto Reg::hasCarry() -> bool {auto tmp = carry_; carry_ = false; return tmp;}
auto Reg::isZero() -> bool {auto tmp = isZero_; isZero_ = false; return tmp;}
auto Reg::hasOverflown() -> bool {auto tmp = overflow_; overflow_ = false; return tmp;}
auto Reg::isNegative() -> bool {auto tmp = isNeg_; isNeg_ = false; return tmp;}
auto Reg::keepCarry() -> void {carry_ = true;}
