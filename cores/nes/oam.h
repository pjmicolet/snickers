#pragma once
#include <array>

struct OAM {
  uint8_t yCord;
  uint8_t tileNum;
  uint8_t attribute;
  uint8_t xCord;
};

using oamTables = std::array<std::array<uint8_t,256>,2>;
