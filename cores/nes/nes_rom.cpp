#include "nes_rom.h"

NesRom::NesRom(const std::filesystem::path &rom_path) : rom_(rom_path) {
  prgrmEnd_ = std::to_integer<size_t>(rom_.at(4))*16384;
}

auto NesRom::getProgram() -> std::span<std::byte> {
  return std::span{rom_.begin()+16,rom_.begin()+16+prgrmEnd_};
}
