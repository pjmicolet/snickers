#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

// Read this https://tessil.github.io/2017/06/22/hat-trie.html

// Primitive implementation, highly inefficient, don't use, but my first attempt so don't judge :)
class SlowTrie {
  using TrieMap = std::unordered_map<char,std::unique_ptr<SlowTrie>>;
  public:
  SlowTrie() {};
  auto insert(std::string_view s) -> void {
    if(s == "")
      return;
    if(data_.find(s[0]) == data_.end()) {
      data_[s[0]] = std::make_unique<SlowTrie>();
    }
    data_[s[0]]->insert(s.substr(1,s.size()-1));
  }

  auto insert(std::vector<std::string>& ss) -> void {
    for(auto& s:ss)
      insert(s);
  }

  auto getMatches(std::string_view s) -> std::vector<std::string> {
    std::vector<std::string> res;
    std::string tmp = "";
    getResultsInternal(s,res,tmp);
    return res;
  }

  SlowTrie(const SlowTrie&) = delete;
  private:
  auto goThroughAll(std::vector<std::string>&res, std::string tmp) -> void {
    if(data_.size() == 0)
        res.push_back(tmp);
    for(auto& entry : data_ ) {
      entry.second->goThroughAll(res,tmp+entry.first);
    }
  };

  auto getResultsInternal(std::string_view s, std::vector<std::string>& res, std::string tmp) -> void {
    if(data_.size() == 0 )
      res.push_back(tmp);
    if(data_.find(s[0]) != data_.end()) {
      tmp += s[0];
      if(s.size() == 1) {
        data_[s[0]]->goThroughAll(res,tmp);
      } else
        data_[s[0]]->getResultsInternal(s.substr(1,s.size()-1),res,tmp);
    }
  }

  private:
    TrieMap data_;
};
