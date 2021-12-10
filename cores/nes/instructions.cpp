#include "instructions.h"
#include <iostream>

void Adc::execute() {
  regs_.A_ = regs_.A_ + regs_.X_;
  std::cout << "OK\n";
}

void And::execute() {
  std::cout << "BOK\n";
}
