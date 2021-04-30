#define RUN_TEST(test)                                                         \
  if (!test) {                                                                 \
    std::cout << #test << " has failed!\n";                                    \
    passed = false;                                                            \
  } else                                                                       \
    std::cout << #test << " has succeeded!\n";
