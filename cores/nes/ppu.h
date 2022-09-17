#pragma once
#include "nes_ram.h"
#include "registers.h"
#include <memory>

using ram_ptr = std::shared_ptr<NES_RAM>;

// PPU is the picture processing unit (renders stuff to screen)

struct PPU {

  PPU(ram_ptr ram);
  auto execute() -> void;
private:
  ram_ptr ram_;
  PPUCTRL ctrlReg_;
  PPUMASK maskReg_;
  const std::byte& statusReg_;
  const std::byte& oamaddrReg_;
  const std::byte& oamDataReg_;
  const std::byte& ppuScrollReg_;
  const std::byte& ppuAddrReg_;
  const std::byte& ppuDataReg_;
  const std::byte& oamdmaReg_;
  uint16_t scanLine_;
};
