#include "ppu.h"

PPU::PPU(ram_ptr ram) : ram_(ram_ptr) {
  ctrlReg_ = PPUCTRL{ram_->load(0x2000)};
  ctrlReg_ = PPUCTRL{ram_->load(0x2001)};
  statusReg_ = ram_->load(0x2002);
  oamaddrReg_ = ram_->load(0x2003);
  oamDataReg_ = ram_->load(0x2004);
  ppuScrollReg_ = ram_->load(0x2005);
  ppuAddReg_ = ram_->load(0x2006);
  ppuDataReg_ = ram_->load(0x2007);
  oamdmaReg_ = ram_->load(0x4014);

  scanLine_ = 0;
}
