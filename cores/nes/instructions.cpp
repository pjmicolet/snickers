#include "instructions.h"
#include <iostream>

Instruction::Instruction(bool debug, CPU_6502& cpu, const std::string string)
    : debug_(debug), cpu_(cpu),  wbc_(cpu.wbc_), regs_(cpu.regs_), branchTaken_(cpu.branchTaken_), name_(string){};

auto Instruction::runInstruction() -> void {
    data_ = static_cast<uint16>(cpu_.dataFetch());
    cpu_.setWriteBackCont();
    if (debug_)
      debug();
    cpu_.cycle();
    execute();
}

auto Instruction::debug() -> void {
  std::cout << name_ << ": " << regs_;
}

auto Instruction::popStack() -> uint8 {
  return cpu_.popStack();
}

auto Instruction::cycleIncrement(uint64_t cycleCount ) -> void {
  return cpu_.cycleIncrement(cycleCount);
}

auto Instruction::pushStack(uint16 data) -> void {
  return cpu_.pushStack(data);
}

auto Instruction::popPCFromStack() -> uint16 {
  return cpu_.popPCFromStack();
}

void Adc::execute() {
  uint8 c = regs_.P_.isCarrySet() ? 1 : 0;
  wbc_ += {data_,c};
  regs_.P_.setCarry(wbc_.hasCarry());
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setOverflow(wbc_.hasOverflown());
}

void Ahx::execute() {}
void Alr::execute() {}
void Anc::execute() {}
void And::execute() {
  wbc_ &= data_;
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}

void Arr::execute() {}

void Asl::execute() {
  bool carry = data_ & 0x80;
  wbc_ = (data_ << 1);
  regs_.P_.setCarry(carry);
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}

void Axs::execute() {}
void Bcc::execute() {
  if(!regs_.P_.isCarrySet()) {
    branchTaken_ = PCIncrementType::BRANCH_TAKEN;
    if(((regs_.PC_ +2 & 0xFF) + static_cast<int8>(data_)) & 0x100)
      cycleIncrement(1);
    regs_.PC_ = regs_.PC_ + data_ + 2;
  }
}

void Bcs::execute() {
  if(regs_.P_.isCarrySet()) {
    branchTaken_ = PCIncrementType::BRANCH_TAKEN;
    if(((regs_.PC_ + 2 & 0xFF) + static_cast<int8>(data_)) & 0x100)
      cycleIncrement(1);
    regs_.PC_ = regs_.PC_ + data_ + 2;
  }
}

void Beq::execute() {
  if(regs_.P_.isZeroSet()) {
    branchTaken_ = PCIncrementType::BRANCH_TAKEN;
    if(((regs_.PC_ + 2 & 0xFF) +  static_cast<int8>(data_)) & 0x100)
      cycleIncrement(1);
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_) + 2;
  }
}

void Bit::execute() {
  uint8 areg = static_cast<uint8>(regs_.A_);
  uint8 res = areg & data_;
  regs_.P_.setZero(res==0);
  regs_.P_.setOverflow(data_&0x40);
  regs_.P_.setNegative(data_&0x80);
}

void Bmi::execute() {
  if(regs_.P_.isNegSet()) {
    branchTaken_ = PCIncrementType::BRANCH_TAKEN;
    if(((regs_.PC_ + 2 & 0xFF) + static_cast<int8>(data_)) & 0x100)
      cycleIncrement(1);
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_) + 2;
  }
}

void Bne::execute() {
  if(!regs_.P_.isZeroSet()) {
    branchTaken_ = PCIncrementType::BRANCH_TAKEN;
    if(((regs_.PC_ + 2 & 0xFF) + static_cast<int8>(data_)) & 0x100)
      cycleIncrement(1);
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_) + 2;
  }
}

void Bpl::execute() {
  if(!regs_.P_.isNegSet()) {
    branchTaken_ = PCIncrementType::BRANCH_TAKEN;
    if(((regs_.PC_ + 2 & 0xFF) + static_cast<int8>(data_)) & 0x100)
      cycleIncrement(1);
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_) + 2;
  }
}

void Brk::execute() {}

void Bvc::execute() {
  if(!regs_.P_.isOverflowSet()) {
    branchTaken_ = PCIncrementType::BRANCH_TAKEN;
    if(((regs_.PC_ + 2 & 0xFF) + static_cast<int8>(data_)) & 0x100)
      cycleIncrement(1);
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_) + 2;
  }
}
void Bvs::execute() {
  if(regs_.P_.isOverflowSet()) {
    branchTaken_ = PCIncrementType::BRANCH_TAKEN;
    if(((regs_.PC_ + 2 & 0xFF) + static_cast<int8>(data_)) & 0x100)
      cycleIncrement(1);
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_) + 2;
  }
}
void Clc::execute() {
  regs_.P_.setCarry(false);
}
void Cld::execute() {
  regs_.P_.setDecimal(false);
}
void Cli::execute() {
  regs_.P_.setInteruptD(false);
}
void Clv::execute() {
  regs_.P_.setOverflow(false);
}
void Cmp::execute() {
  uint8 regA = (uint8)regs_.A_;
  bool equal = regA == data_;
  bool carry = regA >= data_;
  bool neg = (regA - data_) & 0x80;
  regs_.P_.setCarry(carry);
  regs_.P_.setZero(equal);
  regs_.P_.setNegative(neg);
}
void Cpx::execute() {
  //refactor to use wbc ?
  uint8 regX = (uint8)regs_.X_;
  bool equal = regX == data_;
  bool carry = regX >= data_;
  bool neg = (regX - data_) & 0x80;
  regs_.P_.setCarry(carry);
  regs_.P_.setZero(equal);
  regs_.P_.setNegative(neg);
}
void Cpy::execute() {
  uint8 regY = (uint8)regs_.Y_;
  bool equal = regY == data_;
  bool carry = regY >= data_;
  bool neg = (regY - data_) & 0x80;
  regs_.P_.setCarry(carry);
  regs_.P_.setZero(equal);
  regs_.P_.setNegative(neg);
}
void Dcp::execute() {
  uint8 shiftd = data_ - 1;
  wbc_ = shiftd;
  uint8 regA = (uint8)regs_.A_;
  bool equal = regA == shiftd;
  bool carry = regA >= shiftd;
  bool neg = (regA - shiftd) & 0x80;
  regs_.P_.setCarry(carry);
  regs_.P_.setZero(equal);
  regs_.P_.setNegative(neg);
}
void Dec::execute() {
  wbc_ = (data_ - 1);
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}

void Dex::execute() {
  wbc_ -= 1; //fix this
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}
void Dey::execute() {
  wbc_ -= 1;
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}
void Eor::execute() {
  wbc_ ^= data_;
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}
void Inc::execute() {
  wbc_ = (data_ + 1);
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}
void Inx::execute() {
  wbc_ += {1,0};
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}
void Iny::execute() {
  wbc_ += {1,0};
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}
void Isc::execute() {
  uint8 data = data_;
  data++;
  uint8 c = regs_.P_.isCarrySet() ? 1 : 0;
  regs_.A_ -= {data,c};
  regs_.P_.setCarry(regs_.A_.hasCarry());
  regs_.P_.setZero(regs_.A_.isZero());
  regs_.P_.setNegative(regs_.A_.isNegative());
  regs_.P_.setOverflow(regs_.A_.hasOverflown());
  wbc_ = data;
}

void Jmp::execute() {
  branchTaken_ = PCIncrementType::FORCE_JUMP;
  regs_.PC_ = data_;
}

void Jsr::execute() {
  pushStack(regs_.PC_+2);
  regs_.PC_ = data_;
  branchTaken_ = PCIncrementType::FORCE_JUMP_SUBROUTINE;
}

void Kil::execute() {}
void Las::execute() {}

void Lax::execute() {
  regs_.A_ = data_;
  regs_.X_ = data_;
  regs_.P_.setNegative(regs_.A_.isNegative());
  regs_.P_.setZero(regs_.A_.isZero());
}

void Lda::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}

void Ldx::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}

void Ldy::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}

void Lsr::execute() {
  wbc_ >>= 1;
  regs_.P_.setCarry(wbc_.hasCarry());
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}

void Nop::execute() {}

void Ora::execute() {
  wbc_ |= static_cast<uint8>(data_);
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}

void Pha::execute() {
  wbc_ = data_;
}
void Php::execute() {
  wbc_ = regs_.P_.toUint8() | 0b110000;
}
void Pla::execute() {
  wbc_ = popStack();
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Plp::execute() {
  wbc_ = popStack();
}
void Rla::execute() {
  uint8 oldP = regs_.P_.isCarrySet();
  regs_.P_.setCarry(data_&0x80);
  uint8 data = (data_ << 1)|oldP;
  regs_.A_ &= data;
  regs_.P_.setZero(regs_.A_.isZero());
  regs_.P_.setNegative(regs_.A_.isNegative());
  wbc_ = data;
}

void Rra::execute() {
  uint8 oldP = regs_.P_.isCarrySet();
  regs_.P_.setCarry(data_&0x1);
  uint8 data = data_>>1 | oldP << 7;
  uint8 carry = regs_.P_.isCarrySet() ? 1 : 0;
  regs_.A_ += {data,carry};
  regs_.P_.setCarry(regs_.A_.hasCarry());
  regs_.P_.setZero(regs_.A_.isZero());
  regs_.P_.setNegative(regs_.A_.isNegative());
  regs_.P_.setOverflow(regs_.A_.hasOverflown());
  wbc_ = data;
}

void Rol::execute() {
  wbc_.rol(regs_.P_.isCarrySet());
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setCarry(wbc_.hasCarry());
}

void Ror::execute() {
  wbc_.ror(regs_.P_.isCarrySet());
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setCarry(wbc_.hasCarry());
}
void Rti::execute() {
  uint8 statusReg = popStack();
  uint16 pc = popPCFromStack();
  regs_.P_ = statusReg;
  regs_.PC_ = pc;
  branchTaken_ = PCIncrementType::FORCE_JUMP;
  // Takes an extra 4 cycles
}

// You have to mark the branch taken
void Rts::execute() {
  regs_.PC_ = popPCFromStack() + 1;
  branchTaken_ = PCIncrementType::FORCE_JUMP;
  // Takes an extra 4 cycles
}

void Sbc::execute() {
  bool oldC = regs_.P_.isCarrySet();
  uint8 c = regs_.P_.isCarrySet() ? 1 : 0;
  wbc_ -= {data_,c};
  regs_.P_.setCarry(wbc_.hasCarry());
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setOverflow(wbc_.hasOverflown());
}

void SbcMem::execute() {
  bool oldC = regs_.P_.isCarrySet();
  uint8 c = regs_.P_.isCarrySet() ? 1 : 0;
  regs_.A_ -= {data_,c};
  wbc_ = regs_.A_;
  regs_.P_.setCarry(regs_.A_.hasCarry());
  regs_.P_.setZero(regs_.A_.isZero());
  regs_.P_.setNegative(regs_.A_.isNegative());
  regs_.P_.setOverflow(regs_.A_.hasOverflown());
}

void Sax::execute() {
  wbc_ = regs_.A_ & regs_.X_;
}
void Sec::execute() {
  regs_.P_.setCarry(true);
}
void Sed::execute() {
  regs_.P_.setDecimal(true);
}
void Sei::execute() {
  regs_.P_.setInteruptD(true);
}
void Shx::execute() {}
void Shy::execute() {}
void Slo::execute() {
  regs_.P_.setCarry(data_&0x80);
  uint8 data = data_ << 1;
  regs_.A_ |= data;
  regs_.P_.setZero(regs_.A_.isZero());
  regs_.P_.setNegative(regs_.A_.isNegative());
  wbc_ = data;
}
void Sre::execute() {
  regs_.P_.setCarry(data_&0x1);
  uint8 data = data_>>1;
  regs_.A_ ^= data;
  regs_.P_.setZero(regs_.A_.isZero());
  regs_.P_.setNegative(regs_.A_.isNegative());
  wbc_ = data;
}
void Sta::execute() {
  wbc_ = (uint8)regs_.A_; // fix this
}
void Stx::execute() {
  wbc_ = (uint8)regs_.X_;
}
void Sty::execute() {
  wbc_ = (uint8)regs_.Y_;
}
void Tas::execute() {}
void Tax::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Tay::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Tsx::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Txa::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Txs::execute() {
  regs_.S_ = data_ | 0x100;
}
void Tya::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Xaa::execute() {}
