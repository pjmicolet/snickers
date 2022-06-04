#include "../utils/assemblers/asm_utils.h"
#include "../assemblers/nes/nes.h"
#include <iostream>
#include "test_utils.h"

auto test_asm_utils() -> bool {
  bool passed = true;
  static_assert(strSplit<4>("a b cd ef").size() == 4);
  constexpr auto result = strSplit<5>("hey how are you today");
  static_assert(result[0] == "hey");
  static_assert(result[1] == "how");
  static_assert(result[2] == "are");
  static_assert(result[3] == "you");
  static_assert(result[4] == "today");
  static_assert(true == match("@name", "me"));
  static_assert(true == match("@digits", "123"));
  static_assert(true == match("@name @name @name @name", "me myself and i"));
  static_assert(false == match("@digits @name @name @name", "123e myself and i"));
  static_assert(true == match("@digits @name @name @name", "123 myself and i"));
  static_assert(true == match("$(@digits) @name @name @name", "$(123) myself and i"));
  static_assert(true == match("@name #$@digits", "LDA #$32"));
  static_assert(true == match("@name #$@byte", "LDA #$32"));
  static_assert(false == match("@name #$@byte@byte", "LDA #$32"));
  static_assert(true == match("@name #$@byte@byte", "LDA #$32AF"));
  static_assert(false == match("@name #$@byte", "LDA #$32AF"));
  static_assert(true == match("@name #$@byte@digits", "LDA #$32324234123120"));
  return passed;
}

auto test_nes_asm() -> bool {
  bool passed = true;
  NesAssembler a{};
  static_assert(7 == a.getOffsets("ADC")[1] && 8 == a.getOffsets("AND")[0] && 150 == a.getOffsets("TYA")[0]);
  static_assert(0x65 == a.getOpcode("ADC $12"));
  auto vec = a.assemble("ADC $12\nADC $1234");
  BYTE_EQ(vec[0], std::byte(0x65));
  BYTE_EQ(vec[1], std::byte(0x12));
  BYTE_EQ(vec[2], std::byte(0x6D));
  BYTE_EQ(vec[3], std::byte(0x34)); // Watch out fo the stupid endiannes
  BYTE_EQ(vec[4], std::byte(0x12));
  return passed;
}

auto test_asm() -> bool {
  bool passed = true;
  RUN_TEST(test_asm_utils());
  RUN_TEST(test_nes_asm());
  return passed;
}
