#include "../utils/containers/trie.h"
#include "test_utils.h"
#include <vector>
#include <algorithm>
#include <iostream>


auto test_trie() -> bool {
  bool passed = true;
  SlowTrie testTrie{};

  std::vector<std::string> data = {"alpha","beta","agora","bagora"};
  for(auto& str : data){
    testTrie.insert(str);
  }

  auto res = testTrie.getMatches("a");
  std::sort(res.begin(),res.end());
  std::vector<std::string> expected = {"alpha", "agora"};
  std::sort(expected.begin(),expected.end());
  REQUIRE_EQUAL(res[0],expected[0]);
  REQUIRE_EQUAL(res[1],expected[1]);
  res = testTrie.getMatches("ago");
  REQUIRE_EQUAL(res.size(),1);
  REQUIRE_EQUAL(res[0],"agora");
  return passed;
}

auto test_container_utils() -> bool {
  bool passed = true;
  RUN_TEST(test_trie());
  return passed;
}
