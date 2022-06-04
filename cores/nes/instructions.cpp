#include "instructions.h"
#include <iostream>

Instruction::Instruction(bool debug, CPU_6502& cpu, const std::string string)
    : debug_(debug), cpu_(cpu),  wbc_(cpu.wbc_), regs_(cpu.regs_), branchTaken_(cpu.branchTaken_), name_(string){};

auto Instruction::runInstruction() -> void {
    data_ = static_cast<uint16>(cpu_.dataFetch());
    cpu_.setWriteBackCont();
    execute();
    if (debug_)
      debug();
}

auto Instruction::popStack() -> uint8 {
  return cpu_.popStack();
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
  wbc_ = (data_ << 1);
  regs_.P_.setCarry(wbc_.hasCarry());
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}

void Axs::execute() {}
void Bcc::execute() {
  if(!regs_.P_.isCarrySet()) {
    branchTaken_ = true;
    regs_.PC_ = regs_.PC_ + data_;
  }
}
void Bcs::execute() {
  if(regs_.P_.isCarrySet()) {
    branchTaken_ = true;
    regs_.PC_ = regs_.PC_ + data_;
  }
}
void Beq::execute() {
  if(regs_.P_.isZeroSet()) {
    branchTaken_ = true;
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_);
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
    branchTaken_ = true;
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_);
  }
}
void Bne::execute() {
  if(!regs_.P_.isZeroSet()) {
    branchTaken_ = true;
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_);
  }
}
void Bpl::execute() {
  if(!regs_.P_.isNegSet()) {
    branchTaken_ = true;
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_);
  }
}
void Brk::execute() {}
void Bvc::execute() {
  if(!regs_.P_.isOverflowSet()) {
    branchTaken_ = true;
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_);
  }
}
void Bvs::execute() {
  if(regs_.P_.isOverflowSet()) {
    branchTaken_ = true;
    regs_.PC_ = regs_.PC_ + static_cast<int8>(data_);
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
void Dcp::execute() {}
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
void Isc::execute() {}
void Jmp::execute() {
  branchTaken_ = true;
  regs_.PC_ = data_;
}
void Jsr::execute() {
  pushStack(regs_.PC_+2);
  regs_.PC_ = data_;
  branchTaken_ = true;
}
void Kil::execute() {}
void Las::execute() {}
void Lax::execute() {}
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
void Lsr::execute() {}
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
  wbc_ = data_;
}
void Pla::execute() {
  wbc_ = popStack();
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Plp::execute() {
  wbc_ = popStack();
}
void Rla::execute() {}
void Rra::execute() {}
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
void Rti::execute() {}

// Note that you don't mark the branch as taken here
// this is because we push in PC + 2 instead of PC + 3 in JSR
// and so we need the PC to be incremented by 1 afterwards
void Rts::execute() {
  regs_.PC_ = popPCFromStack();
}

void Sbc::execute() {
  uint8 c = regs_.P_.isCarrySet() ? 1 : 0;
  wbc_ -= {data_,c};
  regs_.P_.setCarry(wbc_.hasCarry());
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setOverflow(wbc_.hasOverflown());
}

void Sax::execute() {}
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
void Slo::execute() {}
void Sre::execute() {}
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
void Tsx::execute() {}
void Txa::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Txs::execute() {}
void Tya::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}
void Xaa::execute() {}
