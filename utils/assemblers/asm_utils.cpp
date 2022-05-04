#include "asm_utils.h"

//Non constexpr form :)
auto strSplit(const std::string_view strv, std::string_view delim /*=""*/) -> std::vector<std::string_view> {
  std::vector<std::string_view> output;
  size_t first = 0;
  while(first < strv.size()) {
    const auto second = strv.find_first_of(delim, first);
    if(first == 0 && second == std::string_view::npos)
      output.push_back(strv);
    else if ( first != second ) {
      output.push_back(strv.substr(first,second-first));
    }
    if (second == std::string_view::npos)
      break;
    first = second + 1;
  }
  return output;
}

//This is dirty but it will clear up the string and then create the int
auto stringToInt(const std::string_view pattern) -> int {
  bool foundYet = false;
  int res = 0;
  std::string hex = "";
  for(auto& c : pattern) {
    if(!foundYet){
      if(isHex(c)) {
        foundYet = true;
        hex += c;
      }
    } else{
      if(isHex(c)) {
        hex += c;
      }
      else{break;}
    }
  }
  if(foundYet) {
    res = std::stoi(hex,nullptr,16);
  }
  return res;
}
