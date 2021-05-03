#include <type_traits>

#define RUN_TEST(test)                                                         \
  if (!test) {                                                                 \
    std::cout << #test << " has failed!\n";                                    \
    passed = false;                                                            \
  } else                                                                       \
    std::cout << #test << " has succeeded!\n";

#define REQUIRE_EQUAL(lhval, rhval)                                            \
  if (lhval != rhval) {                                                        \
    std::cout << "\tFailure in test " << __func__ << ":" << __LINE__ << "\n\t"   \
              << lhval << " is not equal to " << rhval << "\n";                \
    passed = false;                                                            \
  }

#define REQUIRE_NOT_EQUAL(lhval, rhval)                                        \
  if (lhval == rhval) {                                                        \
    std::cout << "\tFailure in test " << __func__ << ":" << __LINE__ << "\n\t"   \
              << lhval << " is equal to " << rhval << "\n";                    \
    passed = false;                                                            \
  }

#define STATIC_REQUIRE_EQUAL(lhval, rhval)                                     \
  static_assert(lhval == rhval, "Values must be the same!");\

#define BYTE_EQ(lhs,rhs) REQUIRE_EQUAL(std::to_integer<int>(lhs), std::to_integer<int>(rhs))
#define BYTE_NEQ(lhs,rhs) REQUIRE_NOT_EQUAL(std::to_integer<int>(lhs), std::to_integer<int>(rhs))

