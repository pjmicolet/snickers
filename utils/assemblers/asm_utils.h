#pragma once
#include <array>
#include <string>
#include <iostream>
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

constexpr auto match(const std::string_view pattern, const std::string_view testString) -> bool {
  // formalise some basic pattern match language, maybe something like
  // @name $(@number),Y will be true for LDA $(123),Y
  //const auto splitPattern = strSplit<size>(pattern);
  //auto splitTest = strSplit<size>(testString);
  size_t pattern_index = 0;
  size_t i = 0;

  while(pattern_index < pattern.size() || i < testString.size()) {
    if(pattern[pattern_index] == '@' && pattern[pattern_index+1] == 'n')
    {
      pattern_index = pattern_index+5;
      bool tilEnd = ( pattern_index >= pattern.size() );

      while((tilEnd && i < testString.size() ) || (testString[i] != pattern[pattern_index])){
        if(!(( testString[i] >64 && testString[i]<91) || (testString[i] >96 && testString[i] < 123)))
          return false;
        i++;
      }
      continue;
    } //@digit tag
    else if(pattern[pattern_index] == '@' && pattern[pattern_index+1] == 'd') {
      pattern_index = pattern_index+7;
      bool tilEnd = ( pattern_index >= pattern.size() );

      while((tilEnd && i < testString.size() ) || (testString[i] != pattern[pattern_index])){
        if(testString[i] < 48 || testString[i]>57)
            return false;
        i++;
      }
      continue;

    }
    else if(pattern[pattern_index] != testString[i])
      return false;
    i++;
    pattern_index++;

  }

  return true;
}
