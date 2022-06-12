#include "6502.h"
#include <cstddef>

#define WRAPAROUND(data)\
  data&0xFF;

#define INSINST(name)\
  insts_.emplace_back(std::make_unique<name>(false,*this));

CPU_6502::CPU_6502() {
    std::vector<BankInfo> banks{{0x800, 0}, {0x8, 1}, {0x18, 2}, {0x08, 3}, {0xBFE0, 4}};

    // I know there's 0x10000 addresses but technically
    // these are the only ones that are actually used;
    BanksAndSize cfg = {banks, 0xC808};
    ram_ = std::make_shared<NES_RAM>(cfg);
    wbc_ = WriteBackCont{ram_};
    branchTaken_ = false;
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

inline auto twoByteAddress(ram_ptr& ram, uint16 PC, uint16 offset = 1, bool wrapAround = false) -> uint16 {
  uint16 byteMask = wrapAround ? 0xFF : 0xFFFF;
  auto lowByte = std::to_integer<uint16_t>(ram->load((PC + offset) & byteMask));
  auto highByte = std::to_integer<uint16_t>(ram->load((PC + offset + 1)&byteMask));
  return (uint16)(highByte << 8) | (uint16)lowByte;
}

// What we want is
// address is
// low byte = PC + 1 + X
// high byte = PC + 1 + X + 1
// that gives you where to get the address
// load that
// then you get data from there
inline auto indirectX(ram_ptr& ram, uint16 PC, uint8 X) -> uint16 {
  uint16 baseAddr = (ramToAddress(ram, PC+1) + X) & 0xFF;
  return twoByteAddress(ram,baseAddr,0, true);
}

inline auto indirectY(ram_ptr& ram, uint16 PC, uint8 Y) -> uint16 {
  auto baseAddr = ramToAddress(ram, PC+1);
  auto pcAddr = twoByteAddress(ram, baseAddr, 0, true);
  return pcAddr + Y;
}

inline auto resolveIndirectAddress(ram_ptr& ram, uint16 PC, uint8 offset) -> uint16 {
  auto indirectBytes = twoByteAddress(ram, PC);

  uint16_t highByte = 0;
  // This is due to some bug in the 6502
  if((indirectBytes & 0xFF ) == 0xFF) {
    auto addr = indirectBytes & 0xFF00;
    highByte = std::to_integer<uint16_t>(ram->load(addr));
  }
  else
    highByte = std::to_integer<uint16_t>(ram->load(indirectBytes+1));
  auto lowByte = std::to_integer<uint16_t>(ram->load(indirectBytes));

  return (uint16)(highByte << 8) | (uint16)lowByte;
}

auto CPU_6502::runProgram(size_t until) -> void {
  while(static_cast<size_t>(regs_.PC_) < until){
    execute();
    incrementPC();
  }
}

auto CPU_6502::debugRun(size_t until, std::vector<std::vector<uint16_t>>& data) -> bool {
  CPU_State dbgCheck{};
  size_t line = 0;
  dbgCheck.A.reset(new uint8(data[line][1]));
  dbgCheck.X.reset(new uint8(data[line][2])); // carry is set
  dbgCheck.Y.reset(new uint8(data[line][3])); // carry is set
  dbgCheck.P.reset(new uint8(data[line][4])); // carry is set
  dbgCheck.S.reset(new uint9(data[line][5]+0x100)); // carry is set
  dbgCheck.PC.reset(new uint16(data[line][0])); // carry is set
  size_t numInst = data.size();
  while(static_cast<size_t>(regs_.PC_) < until){
    if(*this == dbgCheck) {
      execute();
      incrementPC();
      line++;
      if(line == numInst)
        return true;
      dbgCheck.A.reset(new uint8(data[line][1]));
      dbgCheck.X.reset(new uint8(data[line][2])); // carry is set
      dbgCheck.Y.reset(new uint8(data[line][3])); // carry is set
      dbgCheck.P.reset(new uint8(data[line][4])); // carry is set
      dbgCheck.S.reset(new uint9(data[line][5]+0x100)); // carry is set
      dbgCheck.PC.reset(new uint16(data[line][0])); // carry is set
    }
    else {
      return false;
    }
  }
  return false;
}

auto CPU_6502::execute() -> void {
  auto inst = getInstructionOp();
  insts_[inst]->runInstruction();
}

// Data fetch takes the PC to figure out what addressing mode the current instruction
// uses and then returns the data that the instruction requires.
// Note that instructions that use indirect address aren't here as that just returns
// an address and not data.
auto CPU_6502::dataFetch() -> uint16 {
  auto addressingMode = resolveAddMode( regs_.PC_ );
  std::byte data;
  switch (addressingMode) {
    break; case IMPL: return 0;//there's no real fetch here afaik
    break; case ACC: return regs_.A_; //there's no real fetch here afaik
    break; case YDATA: return regs_.Y_; //there's no real fetch here afaik
    break; case XDATA: return regs_.X_; //there's no real fetch here afaik
    break; case SDATA: return regs_.S_ & 0xFF; //there's no real fetch here afaik
    break; case IMM: data = ram_->load( regs_.PC_ + 1 );
    break; case ZP: data = ram_->load(ramToAddress(ram_, regs_.PC_+1));
    break; case ZPX: data = ram_->load((ramToAddress(ram_, regs_.PC_+1)+static_cast<uint16>(regs_.X_))&0xFF);
    break; case ZPY: data = ram_->load((ramToAddress(ram_, regs_.PC_+1)+static_cast<uint16>(regs_.Y_))&0xFF);
    break; case REL: data = ram_->load(regs_.PC_+1);
    break; case ABS: data = ram_->load(twoByteAddress(ram_, regs_.PC_));
    break; case ABSADDR: return twoByteAddress(ram_, regs_.PC_);
    break; case ABSX: data = ram_->load(twoByteAddress(ram_, regs_.PC_)+static_cast<uint16>(regs_.X_));
    break; case ABSY: data = ram_->load(twoByteAddress(ram_, regs_.PC_)+static_cast<uint16>(regs_.Y_));
    break; case INDX: data = ram_->load(indirectX(ram_, regs_.PC_,regs_.X_));
    break; case INDY: data = ram_->load(indirectY(ram_, regs_.PC_,regs_.Y_));
    break; case IND:  return resolveIndirectAddress(ram_, regs_.PC_, 0);
    break; default: throw( "This is wrong!" );
  };
  return std::to_integer<uint16_t>(data);
}

auto CPU_6502::setWriteBackCont() -> void {
  wbc_.reset();
  auto destMode = resolveDestMode( regs_.PC_ );
  switch (destMode) {
    break; case AREG:  wbc_.setReg(regs_.A_);
    break; case SREG:  wbc_.setStack(regs_.S_);
    break; case PCREG: wbc_.setReg(regs_.PC_);
    break; case PREG:  wbc_.setReg(regs_.P_);
    break; case YREG:  wbc_.setReg(regs_.Y_);
    break; case XREG:  wbc_.setReg(regs_.X_);
    break; case MEM:   wbc_.setMem(indexFetch());
  };
}

auto CPU_6502::incrementPC() -> void {
  if(branchTaken_) {
    branchTaken_ = false;
    return;
  }
  auto addressingMode = resolveAddMode( regs_.PC_ );
  std::byte data;
  switch (addressingMode) {
    break; case IMPL: case XDATA: case YDATA: case ACC: case SDATA: regs_.PC_ = regs_.PC_ + 1;
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
    break; case ZP:   return ramToAddress(ram_, regs_.PC_+1);
    break; case ZPX:  return WRAPAROUND(ramToAddress(ram_, regs_.PC_+1)+static_cast<uint16>(regs_.X_));
    break; case ZPY:  return WRAPAROUND(ramToAddress(ram_, regs_.PC_+1)+static_cast<uint16>(regs_.Y_));
    break; case REL:  return regs_.PC_+1;
    break; case ABS:  return twoByteAddress(ram_, regs_.PC_);
    break; case ABSX: return twoByteAddress(ram_, regs_.PC_)+static_cast<uint16>(regs_.X_);
    break; case ABSY: return twoByteAddress(ram_, regs_.PC_)+static_cast<uint16>(regs_.Y_);
    break; case INDX: return indirectX(ram_, regs_.PC_,regs_.X_);
    break; case INDY: return indirectY(ram_, regs_.PC_,regs_.Y_);
    break; case IND:  return resolveIndirectAddress(ram_, regs_.PC_, 0);
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
  output += "  Y[" + std::to_string((uint16) regs_.Y_) + "] S["+ std::to_string((uint16)regs_.S_) +"]\n";
  output += "  P[" + regs_.P_.toString() + "]\n";
  output += "  PC:" + std::to_string(regs_.PC_) + "\n";
  return output;
}
