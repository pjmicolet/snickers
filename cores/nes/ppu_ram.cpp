#include "ppu_ram.h"

PPU_RAM::PPU_RAM(BanksAndSize &info, std::vector<int> *tracedLines /*= nullptr*/) noexcept : RAM(info, tracedLines) {}

// Wont check mirrors, but then again we should assume mirrors were setup
// correctly!
auto PPU_RAM::validateRAM(bankIndexPair &pair) const -> void {
  bool bankIssue = (banks_ > 1) && pair.first > (banks_ - 1);
  bool lineIssue = pair.second > (lines_ - 1);

  if (bankIssue || lineIssue) {
    std::string throwMsg = "Trying to access invalid SPPU_RAM line/bank! Bank[" + std::to_string(pair.first) + "]:Line[" + std::to_string(pair.second) +
                           "] only have " + std::to_string(banks_) + " banks and " + std::to_string(lines_) + " lines per bank!";
    throw RamException(throwMsg);
  }
}

auto PPU_RAM::calculateBank(unsigned int index) const noexcept(DONT_THROW) -> bankIndexPair {
  if(index <= 0xFFF) return {0,index}; // PATTERN TABLE 0
  else if(index <= 0x1FFF) return {1,index-0x1000}; // PATTERN TABLE 1
  else if(index <= 0x23FF) return {2,index-0x2000}; // NAMETABLE 0
  else if(index <= 0x27FF) return {3,index-0x2400}; // NAMETABLE 1
  else if(index <= 0x2BFF) return {4,index-0x2800}; // NAMETABLE 2
  else if(index <= 0x2FFF) return {5,index-0x2C00}; // NAMETABLE 3
  else if(index <= 0x33FF) return {2,index - 0x1000}; // NAMETABLE 0 MIRROR
  else if(index <= 0x37FF) return {3,index - 0x1000}; // NAMETABLE 1 MIRROR
  else if(index <= 0x3BFF) return {4,index - 0x1000}; // NAMETABLE 2 MIRROR
  else if(index <= 0x3EFF) return {5,index - 0x1000}; // NAMETABLE 3 MIRROR
  else if(index <= 0x3F1F) return {6,index - 0x3F00}; // Palette RAM
  else return {6, (index - 0x3F00)%0x20 };
}

auto PPU_RAM::addressToBank(const unsigned int address) const noexcept(DONT_THROW) -> unsigned int {
  auto bankIndex = calculateBank(address);
  return bankIndex.first;
}

auto PPU_RAM::store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void {
  auto bankIndex = calculateBank(index);
  auto mirrorBanks = ram_[bankIndex.first].bankMirrors_;
  if (!mirrorBanks.empty()) {
    for (auto &bank : mirrorBanks)
      ram_[bank][bankIndex.second] = data;
  }

  ram_[bankIndex.first][bankIndex.second] = data;
}
