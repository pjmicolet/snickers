#include "../cores/base.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

class RamException : public std::exception {
public:
  RamException(std::string message) : exception_msg_(message){};
  virtual const char *what() const throw() { return exception_msg_.c_str(); }

private:
  std::string exception_msg_;
};

class RAM {
public:
  using Mirrors = std::unordered_map<unsigned int, std::vector<unsigned int>>;
  // Don't want a default constructor because it means very little
  RAM() = delete;
  RAM(unsigned int banks, size_t lines) noexcept : banks_(banks), lines_(lines) {};

  struct RamLine {
    RamLine(int index)
        : bank_(0), data_(std::byte{0}), traced_(false), index_(index){};
    RamLine(int index, int8_t bank)
        : bank_(bank), data_(std::byte{0}), traced_(false), index_(index){};
    RamLine(int index, bool traced)
        : bank_(0), data_(std::byte{0}), traced_(false), index_(index){};
    RamLine(int index, int8_t bank, bool traced)
        : bank_(bank), data_(std::byte{0}), traced_(false), index_(index){};

    friend auto operator<<(std::ostream &os, const RamLine &data)
        -> std::ostream & {
      return os << "RAM[" << std::hex << data.index_ << "]";
    }

    // If we hold a reference to RAM in the RamLine then we could support
    // mirroring using the [] operator
    auto operator=(std::byte data) -> RamLine & {
      if (traced_)
        std::cout << this << std::hex << " = 0x"
                  << std::to_integer<uint16_t>(data_) << " -> 0x"
                  << std::to_integer<uint16_t>(data) << "\n";
      data_ = data;
      return *this;
    }

    auto operator=(RamLine &line) -> RamLine & {
      if (traced_)
        std::cout << this << std::hex << " = 0x"
                  << std::to_integer<uint16_t>(data_) << " -> 0x"
                  << std::to_integer<uint16_t>(line.data_) << "\n";
      data_ = line.data_;
      return *this;
    }

    auto operator==(std::byte data) const -> bool { return data_ == data; }

    auto operator==(const RamLine &line) const -> bool {
      return data_ == line.data_;
    }

    auto load() -> const std::byte & {
      if (traced_)
        std::cout << "Loading " << this << "\n";
      return data_;
    }

    const int8_t bank_;

  protected:
    std::byte data_;

  private:
    bool traced_;
    const int index_;
  };

  struct Bank {
    Bank(size_t numLines, unsigned int bankNum) : bankNum_(bankNum) {
      lines_.reserve(numLines);
    };
    Bank(size_t numLines, unsigned int bankNum,
         std::vector<unsigned int> bankMirrors)
        : bankMirrors_(bankMirrors), bankNum_(bankNum) {
      lines_.reserve(numLines);
    }; // std::move ?

    inline void populate(const unsigned int index, bool traced = false) {
      lines_.emplace_back(index, bankNum_, traced);
    }
    auto operator[](unsigned int index) -> RamLine & { return lines_[index]; }

    std::vector<unsigned int> bankMirrors_;

  private:
    std::vector<RamLine> lines_;
    const unsigned int bankNum_;
  };

  // The main loading function to implement in the specific core memory
  [[nodiscard]] virtual auto load(const unsigned int index) noexcept(DONT_THROW)
      -> const std::byte & = 0;
  virtual auto store(const unsigned int index,
                     const std::byte data) noexcept(DONT_THROW) -> void = 0;
  virtual auto addressToBank(const unsigned int address) const
      noexcept(DONT_THROW) -> unsigned int = 0;

protected:
  // Do we really need this at any point ?
  auto operator[](unsigned int index) noexcept(DONT_THROW) -> RamLine & {
    auto bankIndex = calculateBank(index);
    return ram_[bankIndex.first][bankIndex.second];
  }

  using bankIndexPair = std::pair<unsigned int, unsigned int>;
  virtual auto calculateBank(unsigned int index) const noexcept(DONT_THROW)
      -> bankIndexPair = 0;
  virtual auto validateRAM(bankIndexPair &pair) const -> void = 0;
  std::vector<Bank> ram_;
  const size_t banks_;
  const size_t lines_;
};
