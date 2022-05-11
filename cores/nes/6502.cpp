#include "6502.h"
#include <cstddef>

#define INSINST(name)\
  insts_.emplace_back(std::make_unique<name>(false,*this));

CPU_6502::CPU_6502() {
    std::vector<BankInfo> banks{{0x800, 0}, {0x8, 1}, {0x18, 2}, {0x08, 3}, {0xBFE0, 4}};

    // I know there's 0x10000 addresses but technically
    // these are the only ones that are actually used;
    BanksAndSize cfg = {banks, 0xC808};
    ram_ = std::make_unique<NES_RAM>(cfg);
    bool debug = false;
    INSINST(Brk) INSINST(Ora) INSINST(Kil) INSINST(Slo) INSINST(Nop) INSINST(Ora) INSINST(Asl) INSINST(Slo) INSINST(Php) INSINST(Ora) INSINST(Asl) INSINST(Anc) INSINST(Nop) INSINST(Ora) INSINST(Asl) INSINST(Slo) INSINST(Bpl) INSINST(Ora) INSINST(Kil) INSINST(Slo) INSINST(Nop) INSINST(Ora) INSINST(Asl) INSINST(Slo) INSINST(Clc) INSINST(Ora) INSINST(Nop) INSINST(Slo) INSINST(Nop) INSINST(Ora) INSINST(Asl) INSINST(Slo)
    INSINST(Jsr) INSINST(And) INSINST(Kil) INSINST(Rla) INSINST(Bit) INSINST(And) INSINST(Rol) INSINST(Rla) INSINST(Plp) INSINST(And) INSINST(Rol) INSINST(Anc) INSINST(Bit) INSINST(And) INSINST(Rol) INSINST(Rla) INSINST(Bmi) INSINST(And) INSINST(Kil) INSINST(Rla) INSINST(Nop) INSINST(And) INSINST(Rol) INSINST(Rla) INSINST(Sec) INSINST(And) INSINST(Nop) INSINST(Rla) INSINST(Nop) INSINST(And) INSINST(Rol) INSINST(Rla)
    INSINST(Rti) INSINST(Eor) INSINST(Kil) INSINST(Sre) INSINST(Nop) INSINST(Eor) INSINST(Lsr) INSINST(Sre) INSINST(Pha) INSINST(Eor) INSINST(Lsr) INSINST(Alr) INSINST(Jmp) INSINST(Eor) INSINST(Lsr) INSINST(Sre) INSINST(Bvc) INSINST(Eor) INSINST(Kil) INSINST(Sre) INSINST(Nop) INSINST(Eor) INSINST(Lsr) INSINST(Sre) INSINST(Cli) INSINST(Eor) INSINST(Nop) INSINST(Sre) INSINST(Nop) INSINST(Eor) INSINST(Lsr) INSINST(Sre)
    INSINST(Rts) INSINST(Adc) INSINST(Kil) INSINST(Rra) INSINST(Nop) INSINST(Adc) INSINST(Ror) INSINST(Rra) INSINST(Pla) INSINST(Adc) INSINST(Ror) INSINST(Arr) INSINST(Jmp) INSINST(Adc) INSINST(Ror) INSINST(Rra) INSINST(Bvs) INSINST(Adc) INSINST(Kil) INSINST(Rra) INSINST(Nop) INSINST(Adc) INSINST(Ror) INSINST(Rra) INSINST(Sei) INSINST(Adc) INSINST(Nop) INSINST(Rra) INSINST(Nop) INSINST(Adc) INSINST(Ror) INSINST(Rra)
    INSINST(Nop) INSINST(Sta) INSINST(Nop) INSINST(Sax) INSINST(Sty) INSINST(Sta) INSINST(Stx) INSINST(Sax) INSINST(Dey) INSINST(Nop) INSINST(Txa) INSINST(Xaa) INSINST(Sty) INSINST(Sta) INSINST(Stx) INSINST(Sax) INSINST(Bcc) INSINST(Sta) INSINST(Kil) INSINST(Ahx) INSINST(Sty) INSINST(Sta) INSINST(Stx) INSINST(Sax) INSINST(Tya) INSINST(Sta) INSINST(Txs) INSINST(Tas) INSINST(Shy) INSINST(Sta) INSINST(Shx) INSINST(Ahx)
    INSINST(Ldy) INSINST(Lda) INSINST(Ldx) INSINST(Lax) INSINST(Ldy) INSINST(Lda) INSINST(Ldx) INSINST(Lax) INSINST(Tay) INSINST(Lda) INSINST(Tax) INSINST(Lax) INSINST(Ldy) INSINST(Lda) INSINST(Ldx) INSINST(Lax) INSINST(Bcs) INSINST(Lda) INSINST(Kil) INSINST(Lax) INSINST(Ldy) INSINST(Lda) INSINST(Ldx) INSINST(Lax) INSINST(Clv) INSINST(Lda) INSINST(Tsx) INSINST(Las) INSINST(Ldy) INSINST(Lda) INSINST(Ldx) INSINST(Lax)
    INSINST(Cpy) INSINST(Cmp) INSINST(Nop) INSINST(Dcp) INSINST(Cpy) INSINST(Cmp) INSINST(Dec) INSINST(Dcp) INSINST(Iny) INSINST(Cmp) INSINST(Dex) INSINST(Axs) INSINST(Cpy) INSINST(Cmp) INSINST(Dec) INSINST(Dcp) INSINST(Bne) INSINST(Cmp) INSINST(Kil) INSINST(Dcp) INSINST(Nop) INSINST(Cmp) INSINST(Dec) INSINST(Dcp) INSINST(Cld) INSINST(Cmp) INSINST(Nop) INSINST(Dcp) INSINST(Nop) INSINST(Cmp) INSINST(Dec) INSINST(Dcp)
    INSINST(Cpx) INSINST(Sbc) INSINST(Nop) INSINST(Isc) INSINST(Cpx) INSINST(Sbc) INSINST(Inc) INSINST(Isc) INSINST(Inx) INSINST(Sbc) INSINST(Nop) INSINST(Sbc) INSINST(Cpx) INSINST(Sbc) INSINST(Inc) INSINST(Isc) INSINST(Beq) INSINST(Sbc) INSINST(Kil) INSINST(Isc) INSINST(Nop) INSINST(Sbc) INSINST(Inc) INSINST(Isc) INSINST(Sed) INSINST(Sbc) INSINST(Nop) INSINST(Isc) INSINST(Nop) INSINST(Sbc) INSINST(Inc) INSINST(Isc)
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

auto CPU_6502::runProgram(size_t until) -> void {
  while(static_cast<size_t>(regs_.PC_) < until){
    execute();
    incrementPC();
  }
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

auto CPU_6502::incrementPC() -> void {
  auto addressingMode = resolveAddMode( regs_.PC_ );
  std::byte data;
  switch (addressingMode) {
    break; case IMPL: case ACC: regs_.PC_ = regs_.PC_ + 1;
    break; case IMM: case ZP: case ZPX: case ZPY: case REL: case INDX: case INDY: regs_.PC_ = regs_.PC_ + 2;
    break; case ABS: case ABSX : case ABSY: case IND: regs_.PC_ = regs_.PC_ + 3;
    break; default: throw( "This is wrong!" );
  }
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

/*
 * The formatting needs those extra spaces which is a bit stupid
 */
auto CPU_6502::cliOutput() -> std::string {
  std::string output = "";
  output += "A[" + regs_.A_.toString() + "] X["+ std::to_string((uint16) regs_.X_) +"]\n";
  output += "  Y[" + std::to_string((uint16) regs_.Y_) + "] S["+ regs_.S_.toString() +"]\n";
  output += "  P[" + std::to_string((uint16) regs_.P_) + "]\n";
  output += "  PC:" + std::to_string(regs_.PC_) + "\n";
  return output;
}
