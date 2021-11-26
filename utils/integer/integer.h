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
  constexpr Integer() : bitmask_(createBitmask(n)), size_(n) { value_ = 0; }

  constexpr Integer(const int val) : bitmask_(createBitmask(n)), size_(n) {
    value_ = static_cast<stored_type>( val & static_cast<int>(bitmask_) );
  }

  template <uint8_t size>
  constexpr Integer(const Integer<size> &other)
      : bitmask_(createBitmask(n)), size_(n) {
    value_ = other._getVal();
    value_ &= bitmask_;
  }

  // Don't know if the no discard makes any sense here but don't convert
  // something and do nothing with it !
  [[nodiscard]] constexpr explicit operator stored_type() const noexcept {
    return value_;
  }

  constexpr auto operator=(const int rval) noexcept -> Integer<n> & {
    value_ = static_cast<stored_type>(rval) & bitmask_;
    return *this;
  }

  template <uint8_t otherSize>
  constexpr auto operator=(const Integer<otherSize> &rval) noexcept
      -> Integer<n> & {
    value_ = static_cast<stored_type>(rval) & bitmask_;
    return *this;
  }

  auto operator++(int val) -> Integer<n> {
    Integer<n> res;
    res.value_ = value_++;
    return res;
  }

  constexpr auto operator==(const stored_type& val) const -> bool {
    return val == value_;
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

  constexpr auto bit(int index) const noexcept -> stored_type {
    return (value_ >> index) & 0x1;
  }

  //This is currently the bane of my existence since I don't want to allow users to get the value this way
  constexpr auto _getVal() const noexcept -> stored_type {
    return value_;
  }

private:
  stored_type value_;
  const stored_type bitmask_;
  const uint8_t size_;
};

template<uint8_t n1, uint8_t n2>
auto operator +( const Integer<n1>& lhs, const Integer<n2>& rhs ) {
	std::cout << "Yeh";
	if constexpr ( n1 < n2 )
		return Integer< n2 >(lhs._getVal()+rhs._getVal());
	else 
		return Integer< n1 >(lhs._getVal()+rhs._getVal());
}

