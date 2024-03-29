#include "rom.h"
#include <fstream>

Rom::Rom(const std::filesystem::path &rom_path) {
  if (std::filesystem::exists(rom_path)) {
    if (!load_rom(rom_path)) {
      throw RomException("Found rom but couldn't load");
    }
  } else {
    throw RomException("Invalid rom path");
  }
}

auto Rom::load_rom(const std::filesystem::path &path) -> bool {

  std::ifstream rom(path, std::ios::binary | std::ios::ate);
  // Maybe should throw but idk
  if (!rom)
    return false;

  auto end = rom.tellg();
  rom.seekg(0, std::ios::beg);

  auto size = std::size_t(end - rom.tellg());
  if (size == 0)
    return false;

  data_.resize(size);
  if (!rom.read((char *)data_.data(), static_cast<std::streamsize>(data_.size())))
    return false;

  return true;
}

auto Rom::begin() -> std::vector<std::byte>::iterator {
  return data_.begin();
}

auto Rom::end() -> std::vector<std::byte>::iterator {
  return data_.end();
}

auto Rom::at(size_t index) -> std::byte {
  return data_[index];
}
