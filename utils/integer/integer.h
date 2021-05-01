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

  constexpr auto createBitmask(uint8_t size) -> stored_type {
    stored_type bitmask;
    if constexpr (n == sizeof(stored_type) * 8)
      bitmask = ~0;
    else
      bitmask = (1 << n) - 1;
    return bitmask;
  }

public:
  Integer() : bitmask_(createBitmask(n)), size_(n) { value_ = 0; }

  Integer(int val) : bitmask_(createBitmask(n)), size_(n) {
    stored_type zero = 0;
    value_ = val & bitmask_;
  }

  template <uint8_t size>
  Integer(Integer<size> &other) : bitmask_(createBitmask(n)), size_(n) {
    value_ = other;
    value_ &= bitmask_;
  }

  operator stored_type() const { return value_; }

  auto operator=(const int rval) -> Integer<n> & {
    value_ = static_cast<stored_type>(rval) & bitmask_;
    return *this;
  }

  template <uint8_t otherSize>
  auto operator=(const Integer<otherSize> &rval) -> Integer<n> & {
    value_ = static_cast<stored_type>(rval) & bitmask_;
    return *this;
  }

  auto operator++(int val) -> Integer<n> {
    Integer<n> res;
    res.value_ = value_++;
    return res;
  }

  auto operator--(int val) -> Integer<n> {
    Integer<n> res;
    res.value_ = value_--;
    return res;
  }

  auto operator++() -> Integer<n> & {
    ++value_;
    return *this;
  }

  auto operator--() -> Integer<n> & {
    --value_;
    return *this;
  }

  friend auto operator<<(std::ostream &os, const Integer &integer)
      -> std::ostream & {
    if (integer.size_ <= 8) // this is stupid but it wont print if it's int8_t
      os << static_cast<int16_t>(integer.value_);
    else
      os << integer.value_;
    return os;
  }

  auto bit(int index) -> stored_type { return (value_ >> index) & 0x1; }

private:
  stored_type value_;
  const stored_type bitmask_;
  const uint8_t size_;
};
