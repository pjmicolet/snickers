#include "memory_io.h"

[[nodiscard]] auto CPURamIO::load(const unsigned int index) noexcept(DONT_THROW) -> const std::byte & {
  auto bankIndex = ram_->getBank(index);
  if(bankIndex.first == 1 && bankIndex.second == 0x2) { // SPECIAL CODE FOR PPUSTATUS
    const std::byte flagSet = *nmiOccured_ ? std::byte{0b10000000} : std::byte{0};
    const std::byte mask = std::byte{0x7F};
    *nmiOccured_ = false;
    //Forgive this sin...
    auto& data = const_cast<std::byte&>(ram_->load(index));
    data &= mask;
    data |= flagSet;
    return data;
  }
  return ram_->load(index);
}

auto CPURamIO::store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void {
  auto bankIndex = ram_->getBank(index);
  if(bankIndex.first == 1 && bankIndex.second == 0) {
    *nmiOutput_ = (data & std::byte{0b10000000}) == std::byte{0b10000000};
  }
  if(bankIndex.first == 2 && bankIndex.second == 0x14) {
    *triggeredDMA_ = true;
    uint16_t addr = std::to_integer<uint16_t>(data) << 8;
    for(size_t i =0; i < 256; i++){
      (*oamDMA_)[0][i] = std::to_integer<uint8_t>(load(static_cast<size_t>(addr+i)));
    }
  }
  ram_->store(index, data);
}

auto CPURamIO::clear() -> void {
  ram_->clear();
}

[[nodiscard]] auto PPURamIO::load(const unsigned int index) noexcept(DONT_THROW) -> const std::byte & {
  return ram_->load(index);
}

auto PPURamIO::store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void {
  ram_->store(index, data);
}

MemIO::MemIO() : nmiOccured_(std::make_shared<bool>()), nmiOutput_(std::make_shared<bool>()), triggeredDMA_(std::make_shared<bool>()) {
    std::vector<BankInfo> cpuBanks{{0x800, 0}, {0x8, 1}, {0x18, 2}, {0x08, 3}, {0xBFE0, 4}};
    BanksAndSize cpuCfg = {cpuBanks, 0xC808};
    cpuRam_ = std::make_shared<NES_RAM>(cpuCfg);
    std::vector<BankInfo> ppuBanks{{0x1000, 0}, {0x1000, 1}, {0x400, 2}, {0x400, 3}, {0x400, 4}, {0x400, 5}, {0x20,6}};
    BanksAndSize ppuCfg = {ppuBanks, 0x12320};
    ppuRam_ = std::make_shared<PPU_RAM>(ppuCfg);
}

auto MemIO::getCPURamIO() -> std::shared_ptr<CPURamIO> {
  if(!cpuRamIO_) {
    cpuRamIO_ = std::make_unique<CPURamIO>();
    cpuRamIO_->ram_ = cpuRam_;
    cpuRamIO_->ppuRam_ = ppuRam_;
    cpuRamIO_->nmiOccured_ = nmiOccured_;
    cpuRamIO_->nmiOutput_ = nmiOutput_;
    cpuRamIO_->triggeredDMA_ = triggeredDMA_;
    cpuRamIO_->oamDMA_ = oamDMA_;
  }
  return cpuRamIO_;
}

auto MemIO::getPPURamIO() -> std::shared_ptr<PPURamIO> {
  if(!ppuRamIO_) {
    ppuRamIO_ = std::make_unique<PPURamIO>();
    ppuRamIO_->ram_ = cpuRam_;
    ppuRamIO_->ppuRam_ = ppuRam_;
    ppuRamIO_->nmiOccured_ = nmiOccured_;
    ppuRamIO_->nmiOutput_ = nmiOutput_;
    ppuRamIO_->oamDMA_ = oamDMA_;
  }
  return ppuRamIO_;
}
