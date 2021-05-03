#include <cstddef>
#include <deque>
#include <iostream>
#include <vector>

/*
    Stores data as:
    vec[0] = MSB
    vec[ size - 1 ] = LSB

    Doesn't trim any of the fat,
    so if you stored an 8 bit integer in a 64 bit one then it'll store all the
   extra 0s
*/
template <typename T>
[[nodiscard]] auto integerToByteV(T data) -> std::vector<std::byte> {
  std::vector<std::byte> splitData(sizeof data);

  for (size_t i = 0; i < sizeof(data); i++) {
    splitData[i] = std::byte{(uint8_t)(data & 0xFF)};
    if constexpr (sizeof(data) > 1)
      data >>= 8;
  }
  std::reverse(splitData.begin(), splitData.end());
  return splitData;
}

template <typename T>
[[nodiscard]] auto integerToByteVTrim(T data) -> std::vector<std::byte> {
  std::deque<std::byte> temp;
  for (auto i = 0; i < sizeof data; i++) {
    temp.push_back(std::byte{(uint8_t)(data & 0xFF)});
    data >>= 8;
  }

  while (temp.back() == std::byte{0})
    temp.pop_back();

  std::vector<std::byte> actualVec(temp.rbegin(), temp.rend());

  return actualVec;
}

/*
    Might need to change this but since we have
    MSB in 0 and LSB in size-1
    we still load the vector backwards
    into the return data
*/
template <typename T>
[[nodiscard]] auto byteVecToInteger(std::vector<std::byte> &data) {
  T returnData(0);
  const auto startPos =
      data.size() < sizeof returnData ? 0 : data.size() - sizeof returnData;

  for (size_t i = startPos; i < data.size(); i++) {
    returnData |= std::to_integer<uint8_t>(data[i]);
    if (startPos != data.size() - 1)
      returnData <<= 7;
  }
  return returnData;
}
