#pragma once
#include "../../utils/integer/integer.h"
#include "6502.h"
#include <cstdlib>
#include <iostream>
#include <memory>

struct CPU_6502;
struct Registers;
struct WriteBackCont;

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

using uint8 = Unsigned<8>;
using uint6 = Unsigned<6>;
using uint16 = Unsigned<16>;

#define INST_CLASS(inst_name)                                                  \
  class inst_name : public Instruction {                                       \
  public:                                                                      \
    inst_name(bool debug, CPU_6502& cpu)                                       \
        : Instruction(debug, cpu, #inst_name){};                               \
    virtual ~inst_name() = default;                                            \
                                                                               \
  private:                                                                     \
    void execute() override;                                                   \
  };

/*
** My plan is implement a single derived class for each instruction, don't deal
*with the variants as separate objects or classes
** as that's just unnecessary bloat
*/
class Instruction {
public:
  Instruction(bool debug, CPU_6502& cpu, const std::string string);
  virtual ~Instruction() = default;
  // Each instruction should behave like this, execute then print out whatever
  // custom debugging is asked
public:
  void runInstruction();

protected:
  auto popStack() -> uint8;
  auto pushStack(uint16 data) -> void;
  auto popPCFromStack() -> uint16;
private:
  virtual void execute() {
    std::cerr << "Instruction " << name_ << " has no execute implemented.\n";
    std::exit(EXIT_FAILURE);
  };

  auto debug() -> void;
protected:
  const bool debug_;
  CPU_6502& cpu_;
  WriteBackCont& wbc_;
  Registers& regs_;
  bool& branchTaken_;
  const std::string name_;
  uint16 data_;
private:
  friend struct CPU_6502;
};

INST_CLASS(Adc) INST_CLASS(Ahx) INST_CLASS(Alr) INST_CLASS(Anc)
INST_CLASS(And) INST_CLASS(Arr) INST_CLASS(Asl) INST_CLASS(Axs)
INST_CLASS(Bcc) INST_CLASS(Bcs) INST_CLASS(Beq) INST_CLASS(Bit)
INST_CLASS(Bmi) INST_CLASS(Bne) INST_CLASS(Bpl) INST_CLASS(Brk)
INST_CLASS(Bvc) INST_CLASS(Bvs) INST_CLASS(Clc) INST_CLASS(Cld)
INST_CLASS(Cli) INST_CLASS(Clv) INST_CLASS(Cmp) INST_CLASS(Cpx)
INST_CLASS(Cpy) INST_CLASS(Dcp) INST_CLASS(Dec) INST_CLASS(Dex)
INST_CLASS(Dey) INST_CLASS(Eor) INST_CLASS(Inc) INST_CLASS(Inx)
INST_CLASS(Iny) INST_CLASS(Isc) INST_CLASS(Jmp) INST_CLASS(Jsr)
INST_CLASS(Kil) INST_CLASS(Las) INST_CLASS(Lax) INST_CLASS(Lda)
INST_CLASS(Ldx) INST_CLASS(Ldy) INST_CLASS(Lsr) INST_CLASS(Nop)
INST_CLASS(Ora) INST_CLASS(Pha) INST_CLASS(Php) INST_CLASS(Pla)
INST_CLASS(Plp) INST_CLASS(Rla) INST_CLASS(Rra) INST_CLASS(Rol)
INST_CLASS(Ror) INST_CLASS(Rti) INST_CLASS(Rts) INST_CLASS(Sbc)
INST_CLASS(Sax) INST_CLASS(Sec) INST_CLASS(Sed) INST_CLASS(Sei)
INST_CLASS(Shx) INST_CLASS(Shy) INST_CLASS(Slo) INST_CLASS(Sre)
INST_CLASS(Sta) INST_CLASS(Stx) INST_CLASS(Sty) INST_CLASS(Tas)
INST_CLASS(Tax) INST_CLASS(Tay) INST_CLASS(Tsx) INST_CLASS(Txa)
INST_CLASS(Txs) INST_CLASS(Tya) INST_CLASS(Xaa)
