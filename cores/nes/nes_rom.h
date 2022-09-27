#ifndef NES_ROM_H_
#define NES_ROM_H_
#include "../../components/rom.h"
#include <span>

class NesRom {
  public:
    NesRom(const std::filesystem::path &rom_path);
    auto getProgram() -> std::span<std::byte>;
    auto getChrData() -> std::span<std::byte>;
    auto hasChrData() -> bool;

  private:
    Rom rom_;
    size_t prgrmEnd_;
    size_t chrEnd_;
};

#endif // NES_ROM_H_
