#pragma once
#include "nes_ram.h"
#include "ppu_ram.h"
#include "registers.h"
#include "ppu_palette.h"
#include "oam.h"
#include "memory_io.h"
#include "../../utils/render/sdl2/sdl_window.h"
#include <memory>
#include <string>
#include <array>

using PPURamPtr = std::shared_ptr<PPURamIO>;

namespace PPU_STATICS
{
  static int16_t PRE_RENDER_LINE_END = -1;
  static int16_t PRE_RENDER_LINE_START = 261;
  static int16_t RENDER_LINE_START = 0;
  static int16_t RENDER_LINE_END = 239;
  static int16_t POST_RENDER_LINE = 240;
  static int16_t VBLANK_LINE_START = 241;
  static int16_t VBLANK_LINE_END = 260;
  static int16_t IDLE_CYCLE = 0;
  static int16_t NUM_PIXELS = 256;
  static int16_t NEXT_TILE_DATA_FETCH_START = 257;
  static int16_t NEXT_TWO_TILE_FETCH_CYCLE_START = 321;
  static int16_t STALL_CYCLE_START = 337;
  static int16_t END_OF_HLINE = 340;
};

enum ScanlineStep {
  VISIBLE_SCANLINE,
  PRE_SCANLINE,
  POST_SCANLINE,
  VBLANK_LINES
};

enum HlineStep {
  PIXEL_RENDER,
  SPRITE_FETCH,
  TILE_FETCH,
  BYTE_FETCH,
  IDLE
};

enum Alignment {
  ODD,
  EVEN
};

// Mainly for debugging
struct PatternTableAddr {
  PatternTableAddr(uint16_t addr) {
    yOffset = addr & 0b111;
    bitPlane = (addr & 0b1000) >> 3;
    tileColumn = (addr & 0b11110000)>>4;
    tileRow = (addr & 0b111100000000) >> 8;
    leftOrRightTable = (addr & 0b1000000000000)>>12;
    patternInRange = (addr & 0b10000000000000) >> 13;
  }
  uint8_t yOffset;
  uint8_t bitPlane;
  uint8_t tileColumn;
  uint8_t tileRow;
  uint8_t leftOrRightTable;
  uint8_t patternInRange; //?????
};

// PPU is the picture processing unit (renders stuff to screen)
struct PPU {
  PPU(std::shared_ptr<CPURamIO>(cpuRam), std::shared_ptr<PPURamIO> ram);
  auto cycle(uint64_t cpuCycle) -> void;
  auto writeToRam(size_t addr, std::byte b ) -> void {internalMemory_->store(addr, b);}
  auto getOAMTables() -> oamTables& {return oamdata_;}
//Related to the PPU itself
private:
  PPUCTRL ctrlReg_;
  PPUMASK maskReg_;
  const std::byte& statusReg_;
  const std::byte& oamaddrReg_;
  const std::byte& oamDataReg_;
  const std::byte& ppuScrollReg_;
  const std::byte& ppuAddrReg_;
  const std::byte& ppuDataReg_;
  const std::byte& oamdmaReg_;
  Alignment oddOrEvenFlag_;
  int16_t scanLine_;
  uint64_t ppuCycle_;
  uint64_t pixel_;
  oamTables oamdata_;
  palette nesPalette_;
  PPURamPtr internalMemory_;

//Related to rendering
private:
  snickers::SdlWindow renderWindow_;

private:
  auto getScanlineStep() -> ScanlineStep;
  auto getHlineStep() -> HlineStep;
  auto incrementCycle() -> void;
  auto visibleScanline() -> void;
  auto preScanline() -> void;
  auto postScanline() -> void;
  auto vblankLines() -> void;
};
