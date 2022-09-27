#pragma once
#include "6502.h"
#include "instructions.h"
#include "ppu.h"
#include "nes_rom.h"
#include "nes_ram.h"
#include "memory_io.h"
#include <memory>

auto write_ram(CPU_6502& cpu, std::span<std::byte> bytes, size_t offset = 0) -> void {
  uint16_t index = 0;
  for(auto& byte: bytes) {
    if(index + offset > 0xFFFF)
      break;
    cpu.ram_->store(index+offset,byte);
    index++;
  }
}

auto write_ppu_ram(PPU& ppu, std::span<std::byte> bytes, size_t offset = 0) -> void {
  uint16_t index = 0;
  for(auto& byte: bytes) {
    if(index + offset > 0xFFFF)
      break;
    ppu.writeToRam(index+offset,byte);
    index++;
  }
}

struct NES {
  //       V----- this sets up ram, so need it to default construct first
  NES() : memIO_(), cpu_(memIO_.getCPURamIO()), ppu_(memIO_.getCPURamIO(),memIO_.getPPURamIO()) {}
  CPU_6502 cpu_;
  PPU ppu_;
  MemIO memIO_;
  auto run(std::string rom) -> void {
    NesRom rom_{rom};
    if(rom_.getProgram().size() == 16384) {
        write_ram(cpu_,rom_.getProgram(), 0x8000);
        write_ram(cpu_,rom_.getProgram(), 0xC000);
    } else {
        write_ram(cpu_,rom_.getProgram(), 0x8000);
    }
    if(rom_.hasChrData())
        write_ppu_ram(ppu_,rom_.getChrData(), 0x0000);
    cpu_.initPC();
    while(true) {
        cpu_.step();
        ppu_.cycle(cpu_.getCycleCount());
        cpu_.nmi();
    }
  }
};
