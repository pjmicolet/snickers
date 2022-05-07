#include "../utils/type_utils.h"
#include "test_utils.h"
#include <iostream>

auto test_type_basic() -> bool {
  bool passed = true;
  uint8_t singleByte = 120;
  auto byteArray = integerToByteV<uint8_t>(singleByte);

  REQUIRE_EQUAL(byteArray.size(), 1);

  byteArray = integerToByteV<uint16_t>(singleByte);
  REQUIRE_EQUAL(byteArray.size(), 2);

  byteArray = integerToByteVTrim<uint16_t>(singleByte);
  REQUIRE_EQUAL(byteArray.size(), 1);
  BYTE_EQ(byteArray[0], std::byte{120});

  uint16_t twoBytes = 0xABCD;
  auto twoByteArray = integerToByteVTrim<uint16_t>(twoBytes);
  BYTE_EQ(twoByteArray[0], std::byte{0xAB});
  BYTE_EQ(twoByteArray[1], std::byte{0xCD});

  auto intVer = byteVecToInteger<uint8_t>(byteArray);

  REQUIRE_EQUAL(intVer, 120);

  auto intVer2 = byteVecToInteger<uint16_t>(byteArray);
  REQUIRE_EQUAL(intVer2, 120);

  uint16_t maxBits16 = 0xFFFF;
  byteArray = integerToByteV<uint16_t>(maxBits16);
  BYTE_EQ(byteArray[0], std::byte{0xFF});
  BYTE_EQ(byteArray[1], std::byte{0xFF});

  auto small = byteVecToInteger<uint8_t>(byteArray);
  REQUIRE_EQUAL(small, 0xFF);

  uint32_t fourBytesNotFull = 0xABCD;
  auto fourByteArrayNotFull = integerToByteVTrim<uint32_t>(fourBytesNotFull);
  REQUIRE_EQUAL(fourByteArrayNotFull.size(), 2);
  BYTE_EQ(fourByteArrayNotFull[0], std::byte{0xAB});
  BYTE_EQ(fourByteArrayNotFull[1], std::byte{0xCD});
  auto fourByteArrayFull = integerToByteV<uint32_t>(fourBytesNotFull);
  BYTE_EQ(fourByteArrayFull[0], std::byte{0});
  BYTE_EQ(fourByteArrayFull[1], std::byte{0});
  BYTE_EQ(fourByteArrayFull[2], std::byte{0xAB});
  BYTE_EQ(fourByteArrayFull[3], std::byte{0xCD});

  auto zeroBytes = integerToByteVTrim<uint64_t>(0);
  BYTE_EQ(zeroBytes[0], std::byte{0});

  return passed;
}

auto test_typeutils() -> bool {
  bool passed = true;
  RUN_TEST(test_type_basic());
  return passed;
}
