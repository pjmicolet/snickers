#include "nes_rom.h"

NesRom::NesRom(const std::filesystem::path &rom_path) : rom_(rom_path) {
  prgrmEnd_ = std::to_integer<size_t>(rom_.at(4))*16384;
  chrEnd_ = std::to_integer<size_t>(rom_.at(5))*8192;
}

auto NesRom::getProgram() -> std::span<std::byte> {
  return std::span{rom_.begin()+16,rom_.begin()+16+prgrmEnd_};
}

auto NesRom::hasChrData() -> bool {
  return chrEnd_ != 0;
}

auto NesRom::getChrData() -> std::span<std::byte> {
  auto start = 16+prgrmEnd_+1;
  auto end = start + chrEnd_;
  return std::span{rom_.begin()+start, rom_.begin()+end};
}
