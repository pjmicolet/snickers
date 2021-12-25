#include "../../utils/integer/integer.h"
#include "nes_ram.h"
#include <vector>
#include <memory>

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

struct Registers {
  Registers() : A_(0), X_(0), Y_(0), S_(0), P_(0), PC_(0) {}
  int8 A_;
  int8 X_;
  int8 Y_;
  int8 S_;
  int6 P_;
  int16 PC_;
};

struct CPU_6502 {
  CPU_6502() {
    std::vector<BankInfo> banks {
    {0x800,  0},
    {0x8,    1},
    {0x18,   2},
    {0x08,   3},
    {0xBFE0, 4}
    };

    //I know there's 0x10000 addresses but technically
    //these are the only ones that are actually used;
    BanksAndSize cfg = {banks, 0xC808};
    ram_ = std::make_unique<NES_RAM>(cfg);
  };
  Registers regs_;
  std::unique_ptr<NES_RAM> ram_;
};
