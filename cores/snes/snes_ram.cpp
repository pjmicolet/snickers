#include "snes_ram.h"

SNES_RAM::SNES_RAM(BanksAndSize& info,std::vector<int>* tracedLines /*= nullptr*/) noexcept
: RAM(info,tracedLines) {
}

// Wont check mirrors, but then again we should assume mirrors were setup
// correctly!
auto SNES_RAM::validateRAM(bankIndexPair &pair) const -> void {
  bool bankIssue = (banks_ > 1) && pair.first > (banks_ - 1);
  bool lineIssue = pair.second > (lines_ - 1);

  if (bankIssue || lineIssue) {
    std::string throwMsg =
        "Trying to access invalid SNES_RAM line/bank! Bank[" +
        std::to_string(pair.first) + "]:Line[" + std::to_string(pair.second) +
        "] only have " + std::to_string(banks_) + " banks and " +
        std::to_string(lines_) + " lines per bank!";
    throw RamException(throwMsg);
  }
}

// For SNES we have something like 0xBBIIII
// where BB is the bank number and IIII is the index of the bank we're accessing
auto SNES_RAM::calculateBank(unsigned int index) const noexcept(DONT_THROW)
    -> bankIndexPair {
  auto bank = (index & 0xFF0000) >> 16;
  auto subindex = (index & 0xFFFF);
  auto bankIndex = std::make_pair(bank, subindex);
#if DEBUG
  validateRAM(bankIndex);
#endif
  return bankIndex;
}

auto SNES_RAM::addressToBank(const unsigned int address) const
    noexcept(DONT_THROW) -> unsigned int {
  auto bankIndex = calculateBank(address);
  return bankIndex.first;
}

auto SNES_RAM::store(const unsigned int index,
                     const std::byte data) noexcept(DONT_THROW) -> void {
  auto bankIndex = calculateBank(index);
  auto mirrorBanks = ram_[bankIndex.first].bankMirrors_;
  if (!mirrorBanks.empty()) {
    for (auto &bank : mirrorBanks)
      ram_[bank][bankIndex.second] = data;
  }

  ram_[bankIndex.first][bankIndex.second] = data;
}
