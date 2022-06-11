#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

class RomException : public std::exception {
public:
  RomException(std::string message) : exception_msg_(message){};
  virtual const char *what() const throw() { return exception_msg_.c_str(); }

private:
  std::string exception_msg_;
};

class Rom {
public:
  // For testing only
  Rom(const std::filesystem::path &rom_path);

  Rom(){};

  // Could be in utils idk
  auto load_rom(const std::filesystem::path &path) -> bool;
  auto size() -> size_t { return data_.size(); };
  auto begin() -> std::vector<std::byte>::iterator;
  auto end() -> std::vector<std::byte>::iterator;
  auto at(size_t index) -> std::byte;

private:
  std::vector<std::byte> data_;
};
