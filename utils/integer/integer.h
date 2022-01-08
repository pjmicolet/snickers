#pragma once

#include <cstdint>
#include <iostream>
#include <type_traits>

template <bool is_signed, uint8_t n> class Number {
  using stored_type = std::conditional_t < is_signed == true,
        std::conditional_t<n <= 8, int8_t,
                           std::conditional_t<n <= 16, int16_t, std::conditional_t<n <= 32, int32_t, std::conditional_t<n <= 64, int64_t, void>>>>,
        std::conditional_t<n <= 8, uint8_t,
                           std::conditional_t<n <= 16, uint16_t, std::conditional_t<n <= 32, uint32_t, std::conditional_t<n <= 64, uint64_t, void>>>>>;

  constexpr auto createBitmask(uint8_t size) const -> stored_type {
    stored_type bitmask;
    if constexpr (n == sizeof(stored_type) * 8) {
      if constexpr( is_signed == true )
        bitmask = ~0;
      else
        bitmask = static_cast<stored_type>(~0);
    }
    else
      bitmask = (1 << n) - 1;
    return bitmask;
  }

public:
  constexpr Number() : bitmask_(createBitmask(n)), size_(n) { value_ = 0; }

  constexpr Number(const int val) : bitmask_(createBitmask(n)), size_(n) { value_ = static_cast<stored_type>(val & static_cast<int>(bitmask_)); }

  constexpr Number(const Number<is_signed, n> &other) : bitmask_(createBitmask(n)), size_(n) {
    value_ = other._getVal();
    value_ &= bitmask_;
  }

  // Don't know if the no discard makes any sense here but don't convert
  // something and do nothing with it !
  [[nodiscard]] constexpr operator stored_type() const noexcept { return value_ & bitmask_; }

  // Converting to another integer, if you're larger than it, cut down to
  // whatever it will expect if you're converting to a larger number then just
  // use your bitmask. both bitmasks will generated at compile time. Need to
  // test this more rigourously
  template <uint8_t otherSize>[[nodiscard]] constexpr operator Number<is_signed, otherSize>() const noexcept {
    if constexpr (std::max(n, otherSize) == n)
      return value_ & createBitmask(otherSize);
    else
      return value_ & bitmask_;
  }

  constexpr auto operator=(const int rval) noexcept -> Number<is_signed, n> & {
    value_ = static_cast<stored_type>(rval) & bitmask_;
    return *this;
  }

  constexpr auto operator=(const Number<is_signed, n> &rval) noexcept -> Number<is_signed, n> & {
    value_ = static_cast<stored_type>(rval) & bitmask_;
    return *this;
  }

  auto operator++(int val) -> Number<is_signed, n> {
    Number<is_signed, n> res;
    res.value_ = value_++;
    return res;
  }

  auto operator--(int val) -> Number<is_signed, n> {
    Number<is_signed, n> res;
    res.value_ = value_--;
    return res;
  }

  auto operator++() -> Number<is_signed, n> & {
    ++value_;
    return *this;
  }

  auto operator--() -> Number<is_signed, n> & {
    --value_;
    return *this;
  }

  friend auto operator<<(std::ostream &os, const Number &integer) -> std::ostream & {
    if (integer.size_ <= 8) // this is stupid but it wont print if it's int8_t
      os << static_cast<int16_t>(integer.value_);
    else
      os << integer.value_;
    return os;
  }

  constexpr auto bit(int index) const noexcept -> stored_type { return (value_ >> index) & 0x1; }

  // This is currently the bane of my existence since I don't want to allow
  // users to get the value this way
  constexpr auto _getVal() const noexcept -> stored_type { return value_; }

private:
  stored_type value_;
  const stored_type bitmask_;
  const uint8_t size_;
};

// Need to find a way to get access to the value without it being public
// I want it to be a friend func but I can't get the Barton Nackman trick to
// work
template <bool is_signed, uint8_t n1, uint8_t n2> auto operator+(const Number<is_signed,n1> &lhs, const Number<is_signed,n2> &rhs) {
  return Number<is_signed, std::max(n1, n2)>(lhs._getVal() + rhs._getVal());
}

// Is actually not commutative so this is technically correct
template <bool is_signed, uint8_t n1, uint8_t n2> auto operator-(const Number<is_signed,n1> &lhs, const Number<is_signed,n2> &rhs) {
  return Number<is_signed, std::max(n1, n2)>(lhs._getVal() - rhs._getVal());
}

template <bool is_signed, uint8_t n1, uint8_t n2> auto operator*(const Number<is_signed,n1> &lhs, const Number<is_signed,n2> &rhs) {
  return Number<is_signed, std::max(n1, n2)>(lhs._getVal() * rhs._getVal());
}

template<uint8_t x> using Integer = Number<true,x>;
template<uint8_t x> using Unsigned = Number<false,x>;
