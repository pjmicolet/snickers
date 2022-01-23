#include "nes_ram.h"

NES_RAM::NES_RAM(BanksAndSize &info, std::vector<int> *tracedLines /*= nullptr*/) noexcept : RAM(info, tracedLines) {}

// Wont check mirrors, but then again we should assume mirrors were setup
// correctly!
auto NES_RAM::validateRAM(bankIndexPair &pair) const -> void {
  bool bankIssue = (banks_ > 1) && pair.first > (banks_ - 1);
  bool lineIssue = pair.second > (lines_ - 1);

  if (bankIssue || lineIssue) {
    std::string throwMsg = "Trying to access invalid SNES_RAM line/bank! Bank[" + std::to_string(pair.first) + "]:Line[" + std::to_string(pair.second) +
                           "] only have " + std::to_string(banks_) + " banks and " + std::to_string(lines_) + " lines per bank!";
    throw RamException(throwMsg);
  }
}

auto NES_RAM::calculateBank(unsigned int index) const noexcept(DONT_THROW) -> bankIndexPair {
  if (index <= 0x1FFF)
    return {0, index%0x800};
  else if (index >= 0x2000 && index <= 0x2007) // PPU
    return {1, index - 0x2000};
  else if (index >= 0x2008 && index <= 0x3FFF) //They're all just mirroring
    return {1, (index - 0x2008)%8};
  else if (index >= 0x4000 && index <= 0x4017) //API I/O Reg
    return {2, index - 0x4000};
  else if (index >= 0x4018 && index <= 0x401F) //Disabled APU
    return {3, index - 0x4018};
  return {4, index - 0x4020}; // Cartridge
#if DEBUG
//  validateRAM(bankIndex);
#endif
}

auto NES_RAM::addressToBank(const unsigned int address) const noexcept(DONT_THROW) -> unsigned int {
  auto bankIndex = calculateBank(address);
  return bankIndex.first;
}

auto NES_RAM::store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void {
  auto bankIndex = calculateBank(index);
  auto mirrorBanks = ram_[bankIndex.first].bankMirrors_;
  if (!mirrorBanks.empty()) {
    for (auto &bank : mirrorBanks)
      ram_[bank][bankIndex.second] = data;
  }

  ram_[bankIndex.first][bankIndex.second] = data;
}
