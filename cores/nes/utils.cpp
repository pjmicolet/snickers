#include "utils.h"

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

using uint8 = Unsigned<8>;
using uint9 = Unsigned<9>;
using uint6 = Unsigned<6>;
using uint16 = Unsigned<16>;

WriteBackCont::WriteBackCont() {}

WriteBackCont::WriteBackCont(std::shared_ptr<CPURamIO> ram)  : ram_(ram) {}

auto WriteBackCont::reset() -> void {
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

auto WriteBackCont::setReg(Reg& regptr) -> void { ptr = &regptr; }
auto WriteBackCont::setReg(StatusReg& regptr) -> void { unsetReg(); sptr = &regptr; }
auto WriteBackCont::setReg(uint16& ptr) -> void { unsetReg(); pcptr = &ptr; }
auto WriteBackCont::unsetReg() -> void { ptr = nullptr; sptr = nullptr; stptr = nullptr; }
auto WriteBackCont::setMem(uint16 memAddress) -> void { unsetReg(); memAddr_ = memAddress; }
auto WriteBackCont::setStack(uint9& ptrAddr) -> void { unsetReg(); stptr = &ptrAddr; }

auto WriteBackCont::operator +=(const std::pair<uint8,uint8>dataCarryPair) -> WriteBackCont&{
  if(ptr != nullptr) {
    *ptr += dataCarryPair;
  } else {
    std::cout << "Shouldnt be here\n";
  }
  return *this;
}

auto WriteBackCont::operator -=(const std::pair<uint8,uint8>dataCarryPair) -> WriteBackCont& {
  if(ptr != nullptr) {
    *ptr -= dataCarryPair;
  }
  return *this;
}

auto WriteBackCont::operator -=(int dec) -> WriteBackCont& {
  if(ptr != nullptr) {
    *ptr -= dec;
  }
  return *this;
}

auto WriteBackCont::operator =(const uint8 data) -> WriteBackCont& {
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
  return *this;
}

auto WriteBackCont::operator &=(const uint8 data) -> WriteBackCont& {
  if(ptr != nullptr) {
    *ptr &= data;
  }
  return *this;
}

auto WriteBackCont::operator |=(const uint8 data) -> WriteBackCont& {
  if(ptr != nullptr) {
    *ptr |= data;
  }
  return *this;
}

auto WriteBackCont::operator <<=(const uint8 data) -> WriteBackCont& {
  if(ptr != nullptr) {
    *ptr <<= data;
  } else {
  }
  return *this;
}

auto WriteBackCont::operator >>=(const uint8 data) -> WriteBackCont& {
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

auto WriteBackCont::operator ^=(const uint8 data) -> WriteBackCont& {
  if(ptr != nullptr) {
    *ptr ^= data;
  } else {
    std::cout << "Shouldn't be here\n";
  }
  return *this;
}

auto WriteBackCont::rol(const bool carry) -> WriteBackCont& {
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

auto WriteBackCont::ror(const bool carry) -> WriteBackCont& {
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
auto WriteBackCont::hasCarry() -> bool {
  if(ptr != nullptr)
    return ptr->hasCarry();
  auto tmp = carry_; carry_ = false; return tmp;
}

auto WriteBackCont::isZero() -> bool {
  if(ptr != nullptr)
    return ptr->isZero();
  auto tmp = isZero_; isZero_ = false; return tmp;
}

auto WriteBackCont::hasOverflown() -> bool {
  if(ptr != nullptr)
    return ptr->hasOverflown();
  auto tmp = overflow_; overflow_ = false; return tmp;
}

auto WriteBackCont::isNegative() -> bool {
  if(ptr != nullptr)
    return ptr->isNegative();
  auto tmp = isNeg_; isNeg_ = false; return tmp;
}
