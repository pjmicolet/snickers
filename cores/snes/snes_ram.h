#include "../../components/memory.h"
#include "../base.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

class SNES_RAM : public RAM {
public:
  // Don't want a default constructor because it means very little
  SNES_RAM() = delete;
  SNES_RAM(size_t ramSize, unsigned int banks = 1, Mirrors mirrors = {})
  noexcept;
  SNES_RAM(size_t ramSize, std::vector<int> &tracedLines,
           unsigned int banks = 1)
  noexcept;
  SNES_RAM(size_t ramSize, std::vector<int> &tracedLines,
           unsigned int banks = 1, Mirrors mirrors = {})
  noexcept;

  [[nodiscard]] auto load(const unsigned int index) noexcept(DONT_THROW)
      -> const std::byte & override {
    auto bankIndex = calculateBank(index);
    return ram_[bankIndex.first][bankIndex.second].load();
  };
  auto store(const unsigned int index,
                     const std::byte data) noexcept(DONT_THROW)
      -> void override;
  auto addressToBank(const unsigned int address) const noexcept(DONT_THROW)
      -> unsigned int override;

private:
  // Do we really need this at any point ?
  auto operator[](unsigned int index) noexcept(DONT_THROW) -> RamLine & {
    auto bankIndex = calculateBank(index);
    return ram_[bankIndex.first][bankIndex.second];
  }

  using bankIndexPair = std::pair<unsigned int, unsigned int>;
  auto calculateBank(unsigned int index) const noexcept(DONT_THROW)
      -> bankIndexPair override;
  auto validateRAM(bankIndexPair &pair) const -> void override;
};
