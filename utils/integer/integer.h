#pragma once

#include <cstdint>
#include <iostream>
#include <type_traits>

template <uint8_t n> class Integer {
  using stored_type = std::conditional_t<
      n <= 8, int8_t,
      std::conditional_t<
          n <= 16, int16_t,
          std::conditional_t<n <= 32, int32_t,
                             std::conditional_t<n <= 64, int64_t, void>>>>;

public:
  Integer() {
    value = 0;
    size = n;
    bitmask = (1<<n)-1;
  }

  Integer(int val) {
    size = n;
    stored_type zero = 0;
    bitmask = (1<<n)-1;
    value = val & bitmask;
  }

  operator stored_type() const { return value; }

  auto operator=(const int rval) -> Integer<n> & {
    value = static_cast<stored_type>(rval) & bitmask;
    return *this;
  }

  template <uint8_t otherSize>
  auto operator=(const Integer<otherSize> &rval) -> Integer<n> & {
    value = static_cast<stored_type>(rval) & bitmask;
    return *this;
  }

  auto operator++(int val) -> Integer<n> {
    Integer<n> res;
    res.value = value++;
    return res;
  }

  auto operator--(int val) -> Integer<n> {
    Integer<n> res;
    res.value = value--;
    return res;
  }

  auto operator++() -> Integer<n> & {
    ++value;
    return *this;
  }

  auto operator--() -> Integer<n> & {
    --value;
    return *this;
  }

  friend auto operator<<(std::ostream &os, const Integer &integer)
      -> std::ostream & {
    if (integer.size <= 8) // this is stupid but it wont print if it's int8_t
      os << static_cast<int16_t>(integer.value);
    else
      os << integer.value;
    return os;
  }

private:
  stored_type value;
  stored_type bitmask;
  uint8_t size;
};
