#include "6502.h"
#include "../../utils/integer/integer.h"

class Instruction {
  public:
    Instruction( bool debug, Registers& regs, int64_t op, std::string name ) :
      debug_( debug ),
      regs_( regs ),
      opcode_( op ),
      name_( std::move( name ) ) {};
    virtual void execute() { std::cout << "Shouldn't use default\n"; };
  private:
    virtual void debug() = 0;
  private:
    bool debug_;
    Registers& regs_;
    int64_t opcode_; 
    std::string name_;
};

class Add : Instruction  {
  public:
      void execute() override;
};
