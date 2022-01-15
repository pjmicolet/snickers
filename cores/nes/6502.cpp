#include "6502.h"
#include <cstddef>

inline auto ramToAddress(ram_ptr& ram, uint16 address) -> uint16_t { return std::to_integer<uint16_t>(ram->load(address)); }

inline auto twoByteAddress(ram_ptr& ram, uint16 PC) -> uint16_t {
  auto lowByte = std::to_integer<uint16_t>(ram->load(PC + 1));
  auto highByte = std::to_integer<uint16_t>(ram->load(PC + 2));
  return (uint16_t)(highByte << 8) | (uint16_t)lowByte;
}

inline auto indirectX(ram_ptr& ram, uint16 PC, uint8 X) -> uint16_t {
  auto pcAddr = ramToAddress(ram, PC+1);
  return (uint16_t)((pcAddr+X+1) << 8 ) | ((pcAddr+X) & 0xFF );
}

inline auto indirectY(ram_ptr& ram, uint16 PC, uint8 Y) -> uint16_t {
  auto pcAddr = ramToAddress(ram, PC+1);
  return ( (uint16_t)((pcAddr+1) << 8 ) | ((pcAddr+1) & 0xFF ) + Y );
}

inline auto resolveIndirectAddress(ram_ptr& ram, uint16 PC, uint8 offset) -> uint16_t {
  auto indirectBytes = twoByteAddress(ram, PC);
  auto indirectAddr = (indirectBytes & 0xFF00) | ((indirectBytes + 1) & 0x00FF);

  auto lowByte = indirectBytes;
  auto highByte = indirectAddr;

  return (uint16_t)(highByte << 8) | (uint16_t)lowByte;
}

auto CPU_6502::dataFetch() -> uint8_t {
  auto addressingMode = resolveAddMode( regs_.PC_ );
  std::byte data;
  switch (addressingMode) {
    break; case IMPL: return 0;//there's no real fetch here afaik
    break; case ACC: return 0; //there's no real fetch here afaik
    break; case IMM: data = ram_->load( regs_.PC_ + 1 );
    break; case ZP: data = ram_->load(ramToAddress(ram_, regs_.PC_+1));
    break; case ZPX: data = ram_->load(ramToAddress(ram_, regs_.PC_+1)+regs_.X_);
    break; case ZPY: data = ram_->load(ramToAddress(ram_, regs_.PC_+1)+regs_.Y_);
    break; case REL: data = ram_->load(ramToAddress(ram_, regs_.PC_+1));
    break; case ABS: data = ram_->load(twoByteAddress(ram_, regs_.PC_));
    break; case ABSX: data = ram_->load(twoByteAddress(ram_, regs_.PC_)+regs_.X_);
    break; case ABSY: data = ram_->load(twoByteAddress(ram_, regs_.PC_)+regs_.Y_);
    break; case INDX: data = ram_->load(indirectX(ram_, regs_.PC_,regs_.X_));
    break; case INDY: data = ram_->load(indirectY(ram_, regs_.PC_,regs_.Y_));
    default: throw( "This is wrong!" );
  };
  return std::to_integer<uint8_t>(data);
}

auto CPU_6502::indexFetch() -> uint16_t {
  auto addressingMode = resolveAddMode( regs_.PC_ );
  switch (addressingMode) {
    break; case IND: return resolveIndirectAddress(ram_, regs_.PC_, 0);
    default:
      throw( "There's no other case where we should get indexes" );
  }
  throw( "There's no other case where we should get indexes" );
  return 0;
}
