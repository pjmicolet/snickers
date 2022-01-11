#include "../../utils/integer/integer.h"
#include "nes_ram.h"
#include <memory>
#include <vector>

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

using uint8 = Unsigned<8>;
using uint6 = Unsigned<6>;
using uint16 = Unsigned<16>;

using ram_ptr = std::unique_ptr<NES_RAM>;

struct Registers {
  Registers() : A_(0), X_(0), Y_(0), S_(0), P_(0), PC_(0) {}
  uint8 A_;
  uint8 X_;
  uint8 Y_;
  uint8 S_;
  uint6 P_;
  uint16 PC_;
};

enum NES_ADDRESS_MODE {
  IMPL, // implicit
  ACC,  // accumulator
  IMM,  // immediate
  ZP,   // zero page
  ZPX,  // zero page x
  ZPY,  // zero page y
  REL,  // relative
  ABS,  // absolute
  ABSX, // absolute x
  ABSY, // absolute y
  IND,  // indirect
  INDX, // indexed indirect
  INDY, // indirect indexed
};

struct CPU_6502 {
  CPU_6502() {
    std::vector<BankInfo> banks{{0x800, 0}, {0x8, 1}, {0x18, 2}, {0x08, 3}, {0xBFE0, 4}};

    // I know there's 0x10000 addresses but technically
    // these are the only ones that are actually used;
    BanksAndSize cfg = {banks, 0xC808};
    ram_ = std::make_unique<NES_RAM>(cfg);
  };

  std::unique_ptr<NES_RAM> ram_;
  auto dataFetch() -> uint8_t;


private:
  Registers regs_;

  // clang-format off
  std::array<NES_ADDRESS_MODE, 256> instToAddressMode = {
      IMPL, INDX, IMPL, INDX, ZP,   ZP,   ZP,   ZP,   IMPL, IMM,  IMPL, IMM,  ABS,  ABS,  ABS,  ABS,  ZP,   INDY, IMPL, INDY, INDX, INDX, INDX, INDX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX,
      ABS,  INDX, IMPL, INDX, ZP,   ZP,   ZP,   ZP,   IMPL, IMM,  IMPL, IMM,  ABS,  ABS,  ABS,  ABS,  ZP,   INDY, IMPL, INDY, INDX, INDX, INDX, INDX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX, 
      IMPL, INDX, IMPL, INDX, ZP,   ZP,   ZP,   ZP,   IMPL, IMM,  IMPL, IMM,  ABS,  ABS,  ABS,  ABS,  ZP,   INDY, IMPL, INDY, INDX, INDX, INDX, INDX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX, 
      IMPL, INDX, IMPL, INDX, ZP,   ZP,   ZP,   ZP,   IMPL, IMM,  IMPL, IMM,  ABS,  ABS,  ABS,  ABS,  ZP,   INDY, IMPL, INDY, INDX, INDX, INDX, INDX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX, 
      IMM,  INDX, IMM,  INDX, ZP,   ZP,   ZP,   ZP,   IMPL, IMM,  IMPL, IMM,  ABS,  ABS,  ABS,  ABS,  ZP,   INDY, IMPL, INDY, INDX, INDX, INDX, INDY, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX, 
      IMM,  INDX, IMM,  INDX, ZP,   ZP,   ZP,   ZP,   IMPL, IMM,  IMPL, IMM,  ABS,  ABS,  ABS,  ABS,  ZP,   INDY, IMPL, INDY, INDX, INDX, INDX, INDY, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX, 
      IMM,  INDX, IMM,  INDX, ZP,   ZP,   ZP,   ZP,   IMPL, IMM,  IMPL, IMM,  ABS,  ABS,  ABS,  ABS,  ZP,   INDY, IMPL, INDY, INDX, INDX, INDX, INDX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX, 
      IMM,  INDX, IMM,  INDX, ZP,   ZP,   ZP,   ZP,   IMPL, IMM,  IMPL, IMM,  ABS,  ABS,  ABS,  ABS,  ZP,   INDY, IMPL, INDY, INDX, INDX, INDX, INDX, IMPL, ABSY, IMPL, ABSY, ABSX, ABSX, ABSX, ABSX
  };

  std::array<std::string, 256> instToName = {
      "brk", "ora", "kil", "slo", "nop", "ora", "asl", "slo", "php", "ora", "asl", "anc", "nop", "ora", "asl", "slo", "bpl", "ora", "kil", "slo", "nop", "ora",
      "asl", "slo", "clc", "ora", "nop", "slo", "nop", "ora", "asl", "slo", "jsr", "and", "kil", "rla", "bit", "and", "rol", "rla", "plp", "and", "rol", "anc",
      "bit", "and", "rol", "rla", "bmi", "and", "kil", "rla", "nop", "and", "rol", "rla", "sec", "and", "nop", "rla", "nop", "and", "rol", "rla", "rti", "eor",
      "kil", "sre", "nop", "eor", "lsr", "sre", "pha", "eor", "lsr", "alr", "jmp", "eor", "lsr", "sre", "bvc", "eor", "kil", "sre", "nop", "eor", "lsr", "sre",
      "cli", "eor", "nop", "sre", "nop", "eor", "lsr", "sre", "rts", "adc", "kil", "rra", "nop", "adc", "ror", "rra", "pla", "adc", "ror", "arr", "jmp", "adc",
      "ror", "rra", "bvs", "adc", "kil", "rra", "nop", "adc", "ror", "rra", "sei", "adc", "nop", "rra", "nop", "adc", "ror", "rra", "nop", "sta", "nop", "sax",
      "sty", "sta", "stx", "sax", "dey", "nop", "txa", "xaa", "sty", "sta", "stx", "sax", "bcc", "sta", "kil", "ahx", "sty", "sta", "stx", "sax", "tya", "sta",
      "txs", "tas", "shy", "sta", "shx", "ahx", "ldy", "lda", "ldx", "lax", "ldy", "lda", "ldx", "lax", "tay", "lda", "tax", "lax", "ldy", "lda", "ldx", "lax",
      "bcs", "lda", "kil", "lax", "ldy", "lda", "ldx", "lax", "clv", "lda", "tsx", "las", "ldy", "lda", "ldx", "lax", "cpy", "cmp", "nop", "dcp", "cpy", "cmp",
      "dec", "dcp", "iny", "cmp", "dex", "axs", "cpy", "cmp", "dec", "dcp", "bne", "cmp", "kil", "dcp", "nop", "cmp", "dec", "dcp", "cld", "cmp", "nop", "dcp",
      "nop", "cmp", "dec", "dcp", "cpx", "sbc", "nop", "isc", "cpx", "sbc", "inc", "isc", "inx", "sbc", "nop", "sbc", "cpx", "sbc", "inc", "isc", "beq", "sbc",
      "kil", "isc", "nop", "sbc", "inc", "isc", "sed", "sbc", "nop", "isc", "nop", "sbc", "inc", "isc"};
  // clang-format on

private:
  auto indexFetch() -> uint16_t;

  inline auto resolveAddMode(uint16 PC) -> NES_ADDRESS_MODE {
    auto instruction = ram_->load(PC);
    return instToAddressMode[std::to_integer<size_t>(instruction)];
  }

// Just test helpers
public:
  auto setPC(uint16 pc ) -> void { regs_.PC_ = pc; }
  auto incrementPC() -> void { regs_.PC_++; }
};
