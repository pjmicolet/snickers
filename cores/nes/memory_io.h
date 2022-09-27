#pragma once
#include "nes_ram.h"
#include "ppu_ram.h"
#include "../base.h"

#include <memory>
#include <utility>

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// MemoryIO represents a common class that allows us to handle the interconnection between]
// the CPU memory and the PPU memory for the NES.
//
// This is due to the fact that the NES has some memory-based ports on the CPU side that allow it
// to write to the PPU's memory.
// Rather than having a spaghetti mess of references, the CPU and PPU get access to a member of
// MemoryIO and write through it (they're thin wrappers over the respective memories). This simplifies
// the logic of dealing with writing from one memory to another whilst sharing the logic here.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//   [   MemIO   ]
//       ^ ^
//      /   \
//     v     v
// [ MemCPU MemPPU ]
//


// CPURamIO and PPURamIO both have to expose the same functions as a normal ram (so that from the CPU/PPU's perspective it doesn't look any different)

struct MemIO;

struct CPURamIO {
  CPURamIO() {};
  CPURamIO(NES_RAM& ram, PPU_RAM& ppuRam) : ram_(&ram), ppuRam_(&ppuRam) {};
  [[nodiscard]] auto load(const unsigned int index) noexcept(DONT_THROW) -> const std::byte &;
  auto store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void;
  auto clear() -> void;
  auto getNmiOccured() -> bool { return *nmiOccured_; }
  auto getNmiOutput() -> bool { return *nmiOutput_; }
  auto setNmiOccured(bool b) -> void { *nmiOccured_ = b; }
  auto setNmiOutput(bool b) -> void { *nmiOutput_ = b; }

protected:
  std::shared_ptr<NES_RAM> ram_;
  std::shared_ptr<PPU_RAM> ppuRam_;
  std::shared_ptr<bool> nmiOccured_;
  std::shared_ptr<bool> nmiOutput_;
  std::shared_ptr<bool> triggeredDMA_;
  std::shared_ptr<oamTables> oamDMA_;
private:
  friend class MemIO;
};

struct PPURamIO {
  PPURamIO() {};
  PPURamIO(NES_RAM& ram, PPU_RAM& ppuRam) : ram_(&ram), ppuRam_(&ppuRam) {};
  [[nodiscard]] auto load(const unsigned int index) noexcept(DONT_THROW) -> const std::byte &;
  auto store(const unsigned int index, const std::byte data) noexcept(DONT_THROW) -> void;
  auto getNmiOccured() -> bool { return *nmiOccured_; }
  auto getNmiOutput() -> bool { return *nmiOutput_; }
  auto setNmiOccured(bool b) -> void { *nmiOccured_ = b; }
  auto setNmiOutput(bool b) -> void { *nmiOutput_ = b; }
protected:
  std::shared_ptr<NES_RAM> ram_;
  std::shared_ptr<PPU_RAM> ppuRam_;
  std::shared_ptr<bool> nmiOccured_;
  std::shared_ptr<bool> nmiOutput_;
  std::shared_ptr<oamTables> oamDMA_;
private:
  friend class MemIO;
};

struct MemIO {
  MemIO();
  auto getCPURamIO() -> std::shared_ptr<CPURamIO>;
  auto getPPURamIO() -> std::shared_ptr<PPURamIO>;
private:
  //TODO: does it really have to be all ptrs...
  std::shared_ptr<NES_RAM> cpuRam_;
  std::shared_ptr<PPU_RAM> ppuRam_;
  std::shared_ptr<CPURamIO> cpuRamIO_;
  std::shared_ptr<PPURamIO> ppuRamIO_;
  std::shared_ptr<bool> nmiOccured_;
  std::shared_ptr<bool> nmiOutput_;
  std::shared_ptr<bool> triggeredDMA_;
  std::shared_ptr<oamTables> oamDMA_;
};
