#include "6502.h"
#include <iostream>
#include <cstdlib>
#include "../../utils/integer/integer.h"

#define INST_CLASS(inst_name)\
class inst_name : public Instruction {\
  public:\
    inst_name( bool debug, Registers& regs, int64_t op ) : Instruction( debug, regs, op, #inst_name ) {};\
  private:\
    void execute() override;\
};\

/*
** My plan is implement a single derived class for each instruction, don't deal with the variants as separate objects or classes
** as that's just unnecessary bloat
*/
class Instruction {
  public:
    Instruction( bool debug, Registers& regs, int64_t op, const std::string string ) :
      debug_( debug ),
      regs_( regs ),
      opcode_( op ),
      name_( string ) {};
    // Each instruction should behave like this, execute then print out whatever custom debugging is asked
  public:
    void runInstruction() {
      execute();
      if( debug_ )
        debug();
    };

  private:
    virtual void execute() { std::cerr << "Instruction "<< name_ << " has no execute implemented.\n"; std::exit(EXIT_FAILURE); };
    void debug() {
      std::cout << name_ << " " << opcode_ << "\n";
    };
  protected:
    const bool debug_;
    Registers& regs_;
    int64_t opcode_; 
    const std::string name_;
};

INST_CLASS( Adc )
INST_CLASS( And )
INST_CLASS( Asl )
INST_CLASS( Bcc )
INST_CLASS( Bcs )
INST_CLASS( Beq )
INST_CLASS( Bit )
INST_CLASS( Bmi )
INST_CLASS( Bne )
INST_CLASS( Bpl )
INST_CLASS( Brk )
INST_CLASS( Bvc )
INST_CLASS( Bvs )
INST_CLASS( Clc )
INST_CLASS( Cld )
INST_CLASS( Cli )
INST_CLASS( Clv )
INST_CLASS( Cmp )
INST_CLASS( Cpx )
INST_CLASS( Cpy )
INST_CLASS( Dec )
INST_CLASS( Dex )
INST_CLASS( Dey )
INST_CLASS( Eor )
INST_CLASS( Inc )
INST_CLASS( Inx )
INST_CLASS( Iny )
INST_CLASS( Jmp )
INST_CLASS( Jsr )
INST_CLASS( Lda )
INST_CLASS( Ldx )
INST_CLASS( Ldy )
INST_CLASS( Lsr )
INST_CLASS( Nop )
INST_CLASS( Ora )
INST_CLASS( Pha )
INST_CLASS( Php )
INST_CLASS( Pla )
INST_CLASS( Plp )
INST_CLASS( Rol )
INST_CLASS( Ror )
INST_CLASS( Rti )
INST_CLASS( Rts )
INST_CLASS( Sbc )
INST_CLASS( Sec )
INST_CLASS( Sed )
INST_CLASS( Sei )
INST_CLASS( Sta )
INST_CLASS( Stx )
INST_CLASS( Sty )
INST_CLASS( Tax )
INST_CLASS( Tay )
INST_CLASS( Tsx )
INST_CLASS( Txa )
INST_CLASS( Txs )
INST_CLASS( Tya )
