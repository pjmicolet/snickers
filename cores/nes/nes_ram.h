#pragma once
#include "../../components/memory.h"
#include "../base.h"
#include "oam.h"
#include <memory>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

class NES_RAM : public RAM {
public:
  // Don't want a default constructor because it means very little
  NES_RAM() = delete;
  NES_RAM(BanksAndSize &info, std::vector<int> *tracedLines = nullptr ) noexcept;
  virtual ~NES_RAM() = default;

  [[nodiscard]] auto load(const unsigned int index) noexcept(DONT_THROW) -> const std::byte & override;
  auto store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void override;
  auto addressToBank(const unsigned int address) const noexcept(DONT_THROW) -> unsigned int override;
  auto getSize() -> size_t;

  auto getBank(unsigned int index) const noexcept -> bankIndexPair;

private:
  // Do we really need this at any point ?
  auto operator[](unsigned int index) noexcept(DONT_THROW) -> RamLine & {
    auto bankIndex = calculateBank(index);
    return ram_[bankIndex.first][bankIndex.second];
  }

  using bankIndexPair = std::pair<unsigned int, unsigned int>;
  auto calculateBank(unsigned int index) const noexcept(DONT_THROW) -> bankIndexPair override;
  auto validateRAM(bankIndexPair &pair) const -> void override;
};
