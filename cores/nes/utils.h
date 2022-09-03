#pragma once

#include "../../utils/integer/integer.h"
#include "nes_ram.h"
#include "registers.h"
#include <memory>
#include <vector>

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

using uint8 = Unsigned<8>;
using uint9 = Unsigned<9>;
using uint6 = Unsigned<6>;
using uint16 = Unsigned<16>;

// WriteBackCont is a special container used to hide the details of where instructions
// write back their content.
// The idea is that we can set it up to either be one of the registers or a memory address
// and then simply assign the value.
//
// This way in the instruction we can do:
//
// writeBackContainer = regA + regB;
//      ^
//      |
//      ------- could be a memory address or, let's say, regC
//
// We also have to expose the status flags as there's some specific cases where storing specific values
// into RAM causes the status register to change.
struct WriteBackCont {
  WriteBackCont();

  WriteBackCont(std::shared_ptr<NES_RAM> ram);

  auto reset() -> void;

  auto setReg(Reg& regptr) -> void;
  auto setReg(StatusReg& regptr) -> void;
  auto setReg(uint16& ptr) -> void;
  auto unsetReg() -> void;
  auto setMem(uint16 memAddress) -> void;
  auto setStack(uint9& ptrAddr) -> void;

  auto operator +=(const std::pair<uint8,uint8>dataCarryPair) -> WriteBackCont&;
  auto operator -=(const std::pair<uint8,uint8>dataCarryPair) -> WriteBackCont&;
  auto operator -=(int dec) -> WriteBackCont&;
  auto operator =(const uint8 data) -> WriteBackCont&;
  auto operator &=(const uint8 data) -> WriteBackCont&;
  auto operator |=(const uint8 data) -> WriteBackCont&;
  auto operator <<=(const uint8 data) -> WriteBackCont&;
  auto operator >>=(const uint8 data) -> WriteBackCont&;
  auto operator ^=(const uint8 data) -> WriteBackCont&;

  auto rol(const bool carry) -> WriteBackCont&;
  auto ror(const bool carry) -> WriteBackCont&;

  // I'll eventually regret this
  auto hasCarry() -> bool;
  auto isZero() -> bool;
  auto hasOverflown() -> bool;
  auto isNegative() -> bool;

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

// Used for testing
// It's purpose is to check if the CPU is in a certain state after executing code
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
