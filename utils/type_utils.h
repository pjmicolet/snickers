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

  for (auto i = sizeof(data) - 1;; i--) {
    splitData[i] = std::byte{(uint8_t)(data & 0xFF)};
    data >>= 7;
		if(i == 0)
			break;
  }
  return splitData;
}

template <typename T>
[[nodiscard]] auto integerToByteVTrim(T data) -> std::vector<std::byte> {
  std::deque<std::byte> temp;
  for (auto i = 0; i < sizeof data; i++) {
    temp.push_back(std::byte{(uint8_t)(data & 0xFF)});
    data >>= 7;
  }

  while (temp.back() == std::byte{0})
    temp.pop_back();

  std::vector<std::byte> actualVec(temp.size());
  for (auto i = temp.size() - 1;; i--) {
    actualVec[i] = temp[i];
		if(i == 0)
			break;
	}

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
  auto sizeOfRet = sizeof returnData;
  auto startPos = data.size() - sizeOfRet;

  // Sometimes this happens if we're putting  a smaller thing in a lerger thing
  if (startPos < 0)
    startPos = 0;
  for (size_t i = startPos; i < data.size(); i++) {
    returnData |= std::to_integer<uint8_t>(data[i]);
    if (startPos != data.size() - 1)
      returnData <<= 7;
  }
  return returnData;
}
