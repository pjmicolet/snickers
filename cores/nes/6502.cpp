#include "6502.h"
#include <cstddef>

// These functions are just to handle the different ways of fetching data for 8 and 16 bit data
inline auto ramToAddress(ram_ptr& ram, uint16 address) -> uint16 { return std::to_integer<uint16_t>(ram->load(address)); }

inline auto twoByteAddress(ram_ptr& ram, uint16 PC, uint16 offset = 1) -> uint16 {
  auto lowByte = std::to_integer<uint16_t>(ram->load(PC + offset));
  auto highByte = std::to_integer<uint16_t>(ram->load(PC + offset + 1));
  return (uint16)(highByte << 8) | (uint16)lowByte;
}

inline auto indirectX(ram_ptr& ram, uint16 PC, uint8 X) -> uint16 {
  auto baseAddr = ramToAddress(ram, PC+1) + X;
  return twoByteAddress(ram,baseAddr,0);
}

inline auto indirectY(ram_ptr& ram, uint16 PC, uint8 Y) -> uint16 {
  auto baseAddr = ramToAddress(ram, PC+1);
  auto pcAddr = twoByteAddress(ram, baseAddr, 0);
  return pcAddr + Y;
}

inline auto resolveIndirectAddress(ram_ptr& ram, uint16 PC, uint8 offset) -> uint16 {
  auto indirectBytes = twoByteAddress(ram, PC);
  auto indirectAddr = (indirectBytes & 0xFF00) | ((indirectBytes + 1) & 0x00FF);

  auto lowByte = indirectBytes;
  auto highByte = indirectAddr;

  return (uint16)(highByte << 8) | (uint16)lowByte;
}


// Data fetch takes the PC to figure out what addressing mode the current instruction
// uses and then returns the data that the instruction requires.
// Note that instructions that use indirect address aren't here as that just returns
// an address and not data.
auto CPU_6502::dataFetch() -> uint8 {
  auto addressingMode = resolveAddMode( regs_.PC_ );
  std::byte data;
  switch (addressingMode) {
    break; case IMPL: return 0;//there's no real fetch here afaik
    break; case ACC: return 0; //there's no real fetch here afaik
    break; case IMM: data = ram_->load( regs_.PC_ + 1 );
    break; case ZP: data = ram_->load(ramToAddress(ram_, regs_.PC_+1));
    break; case ZPX: data = ram_->load(ramToAddress(ram_, regs_.PC_+1)+regs_.X_);
    break; case ZPY: data = ram_->load(ramToAddress(ram_, regs_.PC_+1)+regs_.Y_);
    break; case REL: data = ram_->load(regs_.PC_+1);
    break; case ABS: data = ram_->load(twoByteAddress(ram_, regs_.PC_));
    break; case ABSX: data = ram_->load(twoByteAddress(ram_, regs_.PC_)+regs_.X_);
    break; case ABSY: data = ram_->load(twoByteAddress(ram_, regs_.PC_)+regs_.Y_);
    break; case INDX: data = ram_->load(indirectX(ram_, regs_.PC_,regs_.X_));
    break; case INDY: data = ram_->load(indirectY(ram_, regs_.PC_,regs_.Y_));
    break; default: throw( "This is wrong!" );
  };
  return std::to_integer<uint8_t>(data);
}

// Since inddirect addressing returns an address, instructions that use this mode will call indexFetch() rather
// than dataFetch()
auto CPU_6502::indexFetch() -> uint16 {
  auto addressingMode = resolveAddMode( regs_.PC_ );
  switch (addressingMode) {
    break; case IND: return resolveIndirectAddress(ram_, regs_.PC_, 0);
    default:
      throw( "There's no other case where we should get indexes" );
  }
  throw( "There's no other case where we should get indexes" );
  return 0;
}
