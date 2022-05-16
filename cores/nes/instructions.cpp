#include "instructions.h"
#include <iostream>

Instruction::Instruction(bool debug, CPU_6502& cpu, const std::string string)
    : debug_(debug), cpu_(cpu),  wbc_(cpu.wbc_), regs_(cpu.regs_), name_(string){};

auto Instruction::runInstruction() -> void {
    data_ = static_cast<uint16>(cpu_.dataFetch());
    cpu_.setWriteBackCont();
    execute();
    if (debug_)
      debug();
}

void Adc::execute() {
  uint8 c = regs_.P_.isCarrySet() ? 1 : 0;
  wbc_ += {data_,c};
  regs_.P_.setCarry(wbc_.hasCarry());
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setOverflow(wbc_.hasOverflown());
}

void Ahx::execute() {}// std::cout << "Not implemented yet!\n";}
void Alr::execute() {}// std::cout << "Not implemented yet!\n";}
void Anc::execute() {}// std::cout << "Not implemented yet!\n";}
void And::execute() {
  wbc_ &= data_;
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}

void Arr::execute() {}// std::cout << "Not implemented yet!\n";}

void Asl::execute() {
  regs_.P_.setCarry(data_ & 0x80);
  wbc_ = (data_ << 1);
}

void Axs::execute() {}// std::cout << "Not implemented yet!\n";}
void Bcc::execute() {}// std::cout << "Not implemented yet!\n";}
void Bcs::execute() {}// std::cout << "Not implemented yet!\n";}
void Beq::execute() {}// std::cout << "Not implemented yet!\n";}
void Bit::execute() {}// std::cout << "Not implemented yet!\n";}
void Bmi::execute() {}// std::cout << "Not implemented yet!\n";}
void Bne::execute() {}// std::cout << "Not implemented yet!\n";}
void Bpl::execute() {}// std::cout << "Not implemented yet!\n";}
void Brk::execute() {}// std::cout << "Not implemented yet!\n";}
void Bvc::execute() {}// std::cout << "Not implemented yet!\n";}
void Bvs::execute() {}// std::cout << "Not implemented yet!\n";}
void Clc::execute() {
  regs_.P_.setCarry(false);
}// std::cout << "Not implemented yet!\n";}
void Cld::execute() {
  regs_.P_.setDecimal(false);
}// std::cout << "Not implemented yet!\n";}
void Cli::execute() {
  regs_.P_.setInteruptD(false);
}// std::cout << "Not implemented yet!\n";}
void Clv::execute() {}// std::cout << "Not implemented yet!\n";}
void Cmp::execute() {}// std::cout << "Not implemented yet!\n";}
void Cpx::execute() {}// std::cout << "Not implemented yet!\n";}
void Cpy::execute() {}// std::cout << "Not implemented yet!\n";}
void Dcp::execute() {}// std::cout << "Not implemented yet!\n";}
void Dec::execute() {}// std::cout << "Not implemented yet!\n";}
void Dex::execute() {}// std::cout << "Not implemented yet!\n";}
void Dey::execute() {}// std::cout << "Not implemented yet!\n";}
void Eor::execute() {}// std::cout << "Not implemented yet!\n";}
void Inc::execute() {
  wbc_ = (data_ + 1);
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}// std::cout << "Not implemented yet!\n";}
void Inx::execute() {
  wbc_ += {1,0};
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}// std::cout << "Not implemented yet!\n";}
void Iny::execute() {
  wbc_ += {1,0};
  regs_.P_.setZero(wbc_.isZero());
  regs_.P_.setNegative(wbc_.isNegative());
}// std::cout << "Not implemented yet!\n";}
void Isc::execute() {}// std::cout << "Not implemented yet!\n";}
void Jmp::execute() {}// std::cout << "Not implemented yet!\n";}
void Jsr::execute() {}// std::cout << "Not implemented yet!\n";}
void Kil::execute() {}// std::cout << "Not implemented yet!\n";}
void Las::execute() {}// std::cout << "Not implemented yet!\n";}
void Lax::execute() {}// std::cout << "Not implemented yet!\n";}
void Lda::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}// std::cout << "Not implemented yet!\n";}
void Ldx::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}// std::cout << "Not implemented yet!\n";}
void Ldy::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}// std::cout << "Not implemented yet!\n";}
void Lsr::execute() {}// std::cout << "Not implemented yet!\n";}
void Nop::execute() {}// std::cout << "Not implemented yet!\n";}
void Ora::execute() {}// std::cout << "Not implemented yet!\n";}
void Pha::execute() {}// std::cout << "Not implemented yet!\n";}
void Php::execute() {}// std::cout << "Not implemented yet!\n";}
void Pla::execute() {}// std::cout << "Not implemented yet!\n";}
void Plp::execute() {}// std::cout << "Not implemented yet!\n";}
void Rla::execute() {}// std::cout << "Not implemented yet!\n";}
void Rra::execute() {}// std::cout << "Not implemented yet!\n";}
void Rol::execute() {}// std::cout << "Not implemented yet!\n";}
void Ror::execute() {}// std::cout << "Not implemented yet!\n";}
void Rti::execute() {}// std::cout << "Not implemented yet!\n";}
void Rts::execute() {}// std::cout << "Not implemented yet!\n";}
void Sbc::execute() {}// std::cout << "Not implemented yet!\n";}
void Sax::execute() {}// std::cout << "Not implemented yet!\n";}
void Sec::execute() {
  regs_.P_.setCarry(true);
}// std::cout << "Not implemented yet!\n";}
void Sed::execute() {
  regs_.P_.setDecimal(true);
}// std::cout << "Not implemented yet!\n";}
void Sei::execute() {
  regs_.P_.setInteruptD(true);
}// std::cout << "Not implemented yet!\n";}
void Shx::execute() {}// std::cout << "Not implemented yet!\n";}
void Shy::execute() {}// std::cout << "Not implemented yet!\n";}
void Slo::execute() {}// std::cout << "Not implemented yet!\n";}
void Sre::execute() {}// std::cout << "Not implemented yet!\n";}
void Sta::execute() {
  wbc_ = (uint8)regs_.A_; // fix this
}// std::cout << "Not implemented yet!\n";}
void Stx::execute() {
  wbc_ = (uint8)regs_.X_;
}// std::cout << "Not implemented yet!\n";}
void Sty::execute() {
  wbc_ = (uint8)regs_.Y_;
}// std::cout << "Not implemented yet!\n";}
void Tas::execute() {}// std::cout << "Not implemented yet!\n";}
void Tax::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}// std::cout << "Not implemented yet!\n";}
void Tay::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}// std::cout << "Not implemented yet!\n";}
void Tsx::execute() {}// std::cout << "Not implemented yet!\n";}
void Txa::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}// std::cout << "Not implemented yet!\n";}
void Txs::execute() {}// std::cout << "Not implemented yet!\n";}
void Tya::execute() {
  wbc_ = data_;
  regs_.P_.setNegative(wbc_.isNegative());
  regs_.P_.setZero(wbc_.isZero());
}// std::cout << "Not implemented yet!\n";}
void Xaa::execute() {}// std::cout << "Not implemented yet!\n";}
