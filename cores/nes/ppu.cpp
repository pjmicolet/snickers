#include "ppu.h"
#include <iostream>

PPU::PPU(std::shared_ptr<CPURamIO> ram, std::shared_ptr<PPURamIO> ppuRam) : internalMemory_(ppuRam), ctrlReg_(ram->load(0x2000)), maskReg_(ram->load(0x2001)), statusReg_(ram->load(0x2002)), oamaddrReg_(ram->load(0x2003)), oamDataReg_(ram->load(0x2004)), ppuScrollReg_(ram->load(0x2005)), ppuAddrReg_(ram->load(0x2006)), ppuDataReg_(ram->load(0x2007)), oamdmaReg_(ram->load(0x4014)), renderWindow_("NES",256,240) {
  scanLine_ = 0;
  ppuCycle_ = 0;
  oddOrEvenFlag_ = Alignment::EVEN;
  renderWindow_.createWindow();

  nesPalette_ = {   { 0x80, 0x80, 0x80}, { 0x00,0x00,0xBB }, { 0x37,0x00,0xBF }, { 0x84,0x00,0xA6 },
                    { 0xBB,0x00,0x6A }, { 0xB7,0x00,0x1E }, { 0xB3,0x00,0x00 }, { 0x91,0x26,0x00 },
                    { 0x7B,0x2B,0x00 }, { 0x00,0x3E,0x00 }, { 0x00,0x48,0x0D }, { 0x00,0x3C,0x22 },
                    { 0x00,0x2F,0x66 }, { 0x00,0x00,0x00 }, { 0x05,0x05,0x05 }, { 0x05,0x05,0x05 },

                    { 0xC8,0xC8,0xC8 }, { 0x00,0x59,0xFF }, { 0x44,0x3C,0xFF }, { 0xB7,0x33,0xCC },
                    { 0xFF,0x33,0xAA }, { 0xFF,0x37,0x5E }, { 0xFF,0x37,0x1A }, { 0xD5,0x4B,0x00 },
                    { 0xC4,0x62,0x00 }, { 0x3C,0x7B,0x00 }, { 0x1E,0x84,0x15 }, { 0x00,0x95,0x66 },
                    { 0x00,0x84,0xC4 }, { 0x11,0x11,0x11 }, { 0x09,0x09,0x09 }, { 0x09,0x09,0x09 },

                    { 0xFF,0xFF,0xFF }, { 0x00,0x95,0xFF }, { 0x6F,0x84,0xFF }, { 0xD5,0x6F,0xFF },
                    { 0xFF,0x77,0xCC }, { 0xFF,0x6F,0x99 }, { 0xFF,0x7B,0x59 }, { 0xFF,0x91,0x5F },
                    { 0xFF,0xA2,0x33 }, { 0xA6,0xBF,0x00 }, { 0x51,0xD9,0x6A }, { 0x4D,0xD5,0xAE },
                    { 0x00,0xD9,0xFF }, { 0x66,0x66,0x66 }, { 0x0D,0x0D,0x0D }, { 0x0D,0x0D,0x0D },

                    { 0xFF,0xFF,0xFF }, { 0x84,0xBF,0xFF }, { 0xBB,0xBB,0xFF }, { 0xD0,0xBB,0xFF },
                    { 0xFF,0xBF,0xEA }, { 0xFF,0xBF,0xCC }, { 0xFF,0xC4,0xB7 }, { 0xFF,0xCC,0xAE },
                    { 0xFF,0xD9,0xA2 }, { 0xCC,0xE1,0x99 }, { 0xAE,0xEE,0xB7 }, { 0xAA,0xF7,0xEE },
                    { 0xB3,0xEE,0xFF }, { 0xDD,0xDD,0xDD }, { 0x11,0x11,0x11 }, { 0x11,0x11,0x11 }};
}

auto PPU::cycle(uint64_t cpuCycle) -> void {
  // PPU clock is 3* faster than CPU clock
  while(ppuCycle_ <= (cpuCycle*3)) {
    auto scanlineStep = getScanlineStep();
    switch(scanlineStep) {
      break; case ScanlineStep::VISIBLE_SCANLINE: visibleScanline();
      break; case ScanlineStep::PRE_SCANLINE: preScanline();
      break; case ScanlineStep::POST_SCANLINE: postScanline();
      break; case ScanlineStep::VBLANK_LINES: vblankLines();
    }
    incrementCycle();
  }
}

auto PPU::getScanlineStep() -> ScanlineStep {
  if(scanLine_ == PPU_STATICS::PRE_RENDER_LINE_START || scanLine_ == PPU_STATICS::PRE_RENDER_LINE_END)
    return ScanlineStep::PRE_SCANLINE;
  else if(scanLine_ == PPU_STATICS::POST_RENDER_LINE)
    return ScanlineStep::POST_SCANLINE;
  else if(scanLine_ >= PPU_STATICS::VBLANK_LINE_START && scanLine_ <= PPU_STATICS::VBLANK_LINE_END)
    return ScanlineStep::VBLANK_LINES;
  // If we're between 0 and 239
  return ScanlineStep::VISIBLE_SCANLINE;
}

auto PPU::getHlineStep() -> HlineStep {
  if(pixel_ == PPU_STATICS::IDLE_CYCLE)
    return HlineStep::IDLE;
  else if(pixel_ >= PPU_STATICS::NEXT_TILE_DATA_FETCH_START && pixel_ < PPU_STATICS::NEXT_TWO_TILE_FETCH_CYCLE_START)
    return HlineStep::SPRITE_FETCH;
  else if(pixel_ >= PPU_STATICS::NEXT_TWO_TILE_FETCH_CYCLE_START && pixel_ < PPU_STATICS::STALL_CYCLE_START)
    return HlineStep::TILE_FETCH;
  else if(pixel_ >= PPU_STATICS::STALL_CYCLE_START && pixel_ <= PPU_STATICS::END_OF_HLINE)
    return HlineStep::BYTE_FETCH;
  //between 1 and 256;
  return PIXEL_RENDER;
}

//Consider just inlining
auto PPU::incrementCycle() -> void {
  bool frameDone = false;
  ppuCycle_++;
  pixel_++;
  if(pixel_ > PPU_STATICS::END_OF_HLINE) {
    pixel_ = 0;
    scanLine_++;
    // Be sure this is right.
    if(scanLine_ > PPU_STATICS::PRE_RENDER_LINE_START) {
      frameDone = true;
      scanLine_ = -1;
    }
  }
  if(frameDone)
    oddOrEvenFlag_ = oddOrEvenFlag_ == Alignment::ODD ? Alignment::EVEN : Alignment::ODD;
}

auto PPU::preScanline() -> void {
  internalMemory_->setNmiOccured(false);
}

auto PPU::visibleScanline() -> void {
  auto hline = getHlineStep();
  if(hline == HlineStep::IDLE)
    return;
}

auto PPU::postScanline() -> void {

}

auto PPU::vblankLines() -> void {
  internalMemory_->setNmiOccured(false);
}
