#pragma once
#include "../asm_utils.h"
#include <utility>

using opcode = std::pair<std::string_view, uint8_t>;

class NesAssembler {
  public:
  private:
  //Store some table with the opcodes and their pattern matches, then we just scan the right
  //instruction, go through the potential opcode matches and return the opcode
    std::array<std::unique_ptr<OpTable>, 42> ops;
}
