#include "6502.h"
#include <cstddef>

#define INSERT_INST(name)\
  insts_.emplace_back(std::make_unique<name>(debug,*this));

CPU_6502::CPU_6502() {
    std::vector<BankInfo> banks{{0x800, 0}, {0x8, 1}, {0x18, 2}, {0x08, 3}, {0xBFE0, 4}};

    // I know there's 0x10000 addresses but technically
    // these are the only ones that are actually used;
    BanksAndSize cfg = {banks, 0xC808};
    ram_ = std::make_unique<NES_RAM>(cfg);
    bool debug = false;
    INSERT_INST(Brk) INSERT_INST(Ora) INSERT_INST(Kil) INSERT_INST(Slo) INSERT_INST(Nop) INSERT_INST(Ora) INSERT_INST(Asl) INSERT_INST(Slo) INSERT_INST(Php) INSERT_INST(Ora) INSERT_INST(Asl) INSERT_INST(Anc) INSERT_INST(Nop) INSERT_INST(Ora) INSERT_INST(Asl) INSERT_INST(Slo) INSERT_INST(Bpl) INSERT_INST(Ora) INSERT_INST(Kil) INSERT_INST(Slo) INSERT_INST(Nop) INSERT_INST(Ora)
    INSERT_INST(Asl) INSERT_INST(Slo) INSERT_INST(Clc) INSERT_INST(Ora) INSERT_INST(Nop) INSERT_INST(Slo) INSERT_INST(Nop) INSERT_INST(Ora) INSERT_INST(Asl) INSERT_INST(Slo) INSERT_INST(Jsr) INSERT_INST(And) INSERT_INST(Kil) INSERT_INST(Rla) INSERT_INST(Bit) INSERT_INST(And) INSERT_INST(Rol) INSERT_INST(Rla) INSERT_INST(Plp) INSERT_INST(And) INSERT_INST(Rol) INSERT_INST(Anc)
    INSERT_INST(Bit) INSERT_INST(And) INSERT_INST(Rol) INSERT_INST(Rla) INSERT_INST(Bmi) INSERT_INST(And) INSERT_INST(Kil) INSERT_INST(Rla) INSERT_INST(Nop) INSERT_INST(And) INSERT_INST(Rol) INSERT_INST(Rla) INSERT_INST(Sec) INSERT_INST(And) INSERT_INST(Nop) INSERT_INST(Rla) INSERT_INST(Nop) INSERT_INST(And) INSERT_INST(Rol) INSERT_INST(Rla) INSERT_INST(Rti) INSERT_INST(Eor)
    INSERT_INST(Kil) INSERT_INST(Sre) INSERT_INST(Nop) INSERT_INST(Eor) INSERT_INST(Lsr) INSERT_INST(Sre) INSERT_INST(Pha) INSERT_INST(Eor) INSERT_INST(Lsr) INSERT_INST(Alr) INSERT_INST(Jmp) INSERT_INST(Eor) INSERT_INST(Lsr) INSERT_INST(Sre) INSERT_INST(Bvc) INSERT_INST(Eor) INSERT_INST(Kil) INSERT_INST(Sre) INSERT_INST(Nop) INSERT_INST(Eor) INSERT_INST(Lsr) INSERT_INST(Sre)
    INSERT_INST(Cli) INSERT_INST(Eor) INSERT_INST(Nop) INSERT_INST(Sre) INSERT_INST(Nop) INSERT_INST(Eor) INSERT_INST(Lsr) INSERT_INST(Sre) INSERT_INST(Rts) INSERT_INST(Adc) INSERT_INST(Kil) INSERT_INST(Rra) INSERT_INST(Nop) INSERT_INST(Adc) INSERT_INST(Ror) INSERT_INST(Rra) INSERT_INST(Pla) INSERT_INST(Adc) INSERT_INST(Ror) INSERT_INST(Arr) INSERT_INST(Jmp) INSERT_INST(Adc)
    INSERT_INST(Ror) INSERT_INST(Rra) INSERT_INST(Bvs) INSERT_INST(Adc) INSERT_INST(Kil) INSERT_INST(Rra) INSERT_INST(Nop) INSERT_INST(Adc) INSERT_INST(Ror) INSERT_INST(Rra) INSERT_INST(Sei) INSERT_INST(Adc) INSERT_INST(Nop) INSERT_INST(Rra) INSERT_INST(Nop) INSERT_INST(Adc) INSERT_INST(Ror) INSERT_INST(Rra) INSERT_INST(Nop) INSERT_INST(Sta) INSERT_INST(Nop) INSERT_INST(Sax)
    INSERT_INST(Sty) INSERT_INST(Sta) INSERT_INST(Stx) INSERT_INST(Sax) INSERT_INST(Dey) INSERT_INST(Nop) INSERT_INST(Txa) INSERT_INST(Xaa) INSERT_INST(Sty) INSERT_INST(Sta) INSERT_INST(Stx) INSERT_INST(Sax) INSERT_INST(Bcc) INSERT_INST(Sta) INSERT_INST(Kil) INSERT_INST(Ahx) INSERT_INST(Sty) INSERT_INST(Sta) INSERT_INST(Stx) INSERT_INST(Sax) INSERT_INST(Tya) INSERT_INST(Sta)
    INSERT_INST(Txs) INSERT_INST(Tas) INSERT_INST(Shy) INSERT_INST(Sta) INSERT_INST(Shx) INSERT_INST(Ahx) INSERT_INST(Ldy) INSERT_INST(Lda) INSERT_INST(Ldx) INSERT_INST(Lax) INSERT_INST(Ldy) INSERT_INST(Lda) INSERT_INST(Ldx) INSERT_INST(Lax) INSERT_INST(Tay) INSERT_INST(Lda) INSERT_INST(Tax) INSERT_INST(Lax) INSERT_INST(Ldy) INSERT_INST(Lda) INSERT_INST(Ldx) INSERT_INST(Lax)
    INSERT_INST(Bcs) INSERT_INST(Lda) INSERT_INST(Kil) INSERT_INST(Lax) INSERT_INST(Ldy) INSERT_INST(Lda) INSERT_INST(Ldx) INSERT_INST(Lax) INSERT_INST(Clv) INSERT_INST(Lda) INSERT_INST(Tsx) INSERT_INST(Las) INSERT_INST(Ldy) INSERT_INST(Lda) INSERT_INST(Ldx) INSERT_INST(Lax) INSERT_INST(Cpy) INSERT_INST(Cmp) INSERT_INST(Nop) INSERT_INST(Dcp) INSERT_INST(Cpy) INSERT_INST(Cmp)
    INSERT_INST(Dec) INSERT_INST(Dcp) INSERT_INST(Iny) INSERT_INST(Cmp) INSERT_INST(Dex) INSERT_INST(Axs) INSERT_INST(Cpy) INSERT_INST(Cmp) INSERT_INST(Dec) INSERT_INST(Dcp) INSERT_INST(Bne) INSERT_INST(Cmp) INSERT_INST(Kil) INSERT_INST(Dcp) INSERT_INST(Nop) INSERT_INST(Cmp) INSERT_INST(Dec) INSERT_INST(Dcp) INSERT_INST(Cld) INSERT_INST(Cmp) INSERT_INST(Nop) INSERT_INST(Dcp)
    INSERT_INST(Nop) INSERT_INST(Cmp) INSERT_INST(Dec) INSERT_INST(Dcp) INSERT_INST(Cpx) INSERT_INST(Sbc) INSERT_INST(Nop) INSERT_INST(Isc) INSERT_INST(Cpx) INSERT_INST(Sbc) INSERT_INST(Inc) INSERT_INST(Isc) INSERT_INST(Inx) INSERT_INST(Sbc) INSERT_INST(Nop) INSERT_INST(Sbc) INSERT_INST(Cpx) INSERT_INST(Sbc) INSERT_INST(Inc) INSERT_INST(Isc) INSERT_INST(Beq) INSERT_INST(Sbc)
    INSERT_INST(Kil) INSERT_INST(Isc) INSERT_INST(Nop) INSERT_INST(Sbc) INSERT_INST(Inc) INSERT_INST(Isc) INSERT_INST(Sed) INSERT_INST(Sbc) INSERT_INST(Nop) INSERT_INST(Isc) INSERT_INST(Nop) INSERT_INST(Sbc) INSERT_INST(Inc) INSERT_INST(Isc)
  }

// These functions are just to handle the different ways of fetching data for 8 and 16 bit data
inline auto ramToAddress(ram_ptr& ram, uint16 address) -> uint16 { return std::to_integer<uint16_t>(ram->load(address)); }

inline auto twoByteAddress(ram_ptr& ram, uint16 PC, uint16 offset = 1) -> uint16 {
  auto lowByte = std::to_integer<uint16_t>(ram->load(PC + offset));
  auto highByte = std::to_integer<uint16_t>(ram->load(PC + offset + 1));
  return (uint16)(highByte << 8) | (uint16)lowByte;
}

inline auto indirectX(ram_ptr& ram, uint16 PC, uint8 X) -> uint16 {
  uint16 baseAddr = ramToAddress(ram, PC+1) + X;
  return twoByteAddress(ram,baseAddr,0);
}

inline auto indirectY(ram_ptr& ram, uint16 PC, uint8 Y) -> uint16 {
  auto baseAddr = ramToAddress(ram, PC+1);
  auto pcAddr = twoByteAddress(ram, baseAddr, 0);
  return pcAddr + Y;
}

inline auto resolveIndirectAddress(ram_ptr& ram, uint16 PC, uint8 offset) -> uint16 {
  auto indirectBytes = twoByteAddress(ram, PC);
  uint16 indirectAddr = (indirectBytes & 0xFF00) | ((indirectBytes + 1) & 0x00FF);

  auto lowByte = indirectBytes;
  auto highByte = indirectAddr;

  return (uint16)(highByte << 8) | (uint16)lowByte;
}

auto CPU_6502::execute() -> void {
  auto inst = getInstructionOp();
  insts_[inst]->runInstruction();
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

auto CPU_6502::printDebug() -> void {
  std::cout << regs_;
}
