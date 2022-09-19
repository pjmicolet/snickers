#include "memory_io.h"

[[nodiscard]] auto CPURamIO::load(const unsigned int index) noexcept(DONT_THROW) -> const std::byte & {
  return ram_->load(index);
}

auto CPURamIO::store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void {
  ram_->store(index, data);
}

[[nodiscard]] auto PPURamIO::load(const unsigned int index) noexcept(DONT_THROW) -> const std::byte & {
  return ram_->load(index);
}

auto PPURamIO::store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void {
  ram_->store(index, data);
}

MemIO::MemIO() {
    std::vector<BankInfo> cpuBanks{{0x800, 0}, {0x8, 1}, {0x18, 2}, {0x08, 3}, {0xBFE0, 4}};
    BanksAndSize cpuCfg = {cpuBanks, 0xC808};
    cpuRam_ = std::make_shared<NES_RAM>(cpuCfg,nmiOccured_,nmiOutput_, triggeredDMA_);
    std::vector<BankInfo> ppuBanks{{0x1000, 0}, {0x1000, 1}, {0x400, 2}, {0x400, 3}, {0x400, 4}, {0x400, 5}, {0x20,6}};
    BanksAndSize ppuCfg = {ppuBanks, 0x12320};
    ppuRam_ = std::make_shared<PPU_RAM>(ppuCfg);
}

auto MemIO::getCPURamIO() -> const CPURamIO& {
  if(cpuRamIO_.ram_ == nullptr) {
    cpuRamIO_.ram_ = cpuRam_;
    cpuRamIO_.ppuRam_ = ppuRam_;
  }
  return cpuRamIO_;
}

auto MemIO::getPPURamIO() -> const PPURamIO& {
  if(ppuRamIO_.ram_ == nullptr) {
    ppuRamIO_.ram_ = cpuRam_;
    ppuRamIO_.ppuRam_ = ppuRam_;
  }
  return ppuRamIO_;
}
