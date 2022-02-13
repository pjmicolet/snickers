#pragma once
#include <array>
#include <string>
#include <string_view>

// Taken from cppstories.com 2021, some interesting blogpost on string and vector becoming constexpr
template<int size>
constexpr auto strSplit(const std::string_view strv, std::string_view delim = " ") -> std::array<std::string_view,size> {
  std::array<std::string_view, size> output;
  size_t first = 0;
  size_t index = 0;
  while(first < strv.size()) {
    const auto second = strv.find_first_of(delim, first);
    if ( first != second ) {
      output[index] = strv.substr(first,second-first);
      index++;
    }
    if (second == std::string_view::npos)
      break;
    first = second + 1;
  }
  return output;
}

template<int size>
constexpr auto match(const std::string_view pattern, const std::string_view testString) -> bool {
  // formalise some basic pattern match language, maybe something like
  // @name $(@number),Y will be true for LDA $(123),Y
  const auto splitPattern = strSplit<size>(pattern);
  auto splitTest = strSplit<size>(testString);
  for(size_t i =0;i < (size_t)size; i++){
    //We have a label
    if(splitPattern[i] == "@name")
    {
      if(splitTest[i].find_first_of("123456789") != std::string_view::npos){
        return false;
      }
      // Not sure this will ever genuinely be needed
    }
    if(splitPattern[i] == "@digits") {
      if(splitTest[i].find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHHIJKLMNOPQRSTUVWXYZ") != std::string_view::npos )
          return false;
    }
  }
  return true;
}
