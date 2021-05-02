#define RUN_TEST(test)                                                         \
  if (!test) {                                                                 \
    std::cout << #test << " has failed!\n";                                    \
    passed = false;                                                            \
  } else                                                                       \
    std::cout << #test << " has succeeded!\n";

#define REQUIRE_EQUAL(lhval, rhval)                                            \
  if (lhval != rhval) {                                                        \
    std::cout << "Failure in test " << __func__ << ":" << __LINE__ << " "      \
              << lhval << " is not equal to " << rhval << "\n";                \
    passed = false;                                                            \
  }

#define STATIC_REQUIRE_EQUAL(lhval, rhval)                                     \
  static_assert(lhval == rhval, "Values must be the same!");\
