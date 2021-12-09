#include "../../utils/integer/integer.h"

using int8 = Integer<8>;
using int6 = Integer<6>;
using int16 = Integer<16>;

struct Registers {
  Registers() : A_(0), X_(0), Y_(0), S_(0), P_(0), PC_(0) {}
  int8 A_;
  int8 X_;
  int8 Y_;
  int8 S_;
  int6 P_;
  int16 PC_;
};
