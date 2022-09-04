#pragma once
#include "nes_ram.h"
#include "registers.h"

using ram_ptr = std::shared_ptr<NES_RAM>;

// PPU is the picture processing unit (renders stuff to screen)

struct PPU {

  PPU(ram_ptr ram);
  auto execute() -> void;
private:
  PPUCTRL ctrlReg_;
  PPUMASK maskReg_;
  std::byte& statusReg_;
  std::byte& oamaddrReg_;
  std::byte& oamDataReg_;
  std::byte& ppuScrollReg_;
  std::byte& ppuAddrReg_;
  std::byte& ppuDataReg_;
  std::byte& oamdmaReg_;
  ram_ptr ram_;
  uint16_t scanLine_;
}
