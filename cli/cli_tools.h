#pragma once
#include <stdlib.h>
#include <curses.h>
#include <string>
#include <signal.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include "../utils/containers/trie.h"

template<typename T>
using windowPtr = std::unique_ptr<T,std::function<void(T*)>>;

struct SnickersWindow {

  SnickersWindow(int y, int x, int numRows, int numCols) :
    inity_(y), initx_(x), wind_(newwin(numRows,numCols,y,x),[](WINDOW* w) {delwin(w);}), textData_(""), yOffset_(0), xOffset_(0) {
  };

  SnickersWindow(int y, int x, int numRows, int numCols, int yOffset, int xOffset) :
    inity_(y), initx_(x), wind_(newwin(numRows,numCols,y,x),[](WINDOW* w) {delwin(w);}), textData_(""), yOffset_(yOffset), xOffset_(xOffset) {
  };

  virtual auto clear(bool moveToStart) -> void = 0;
  virtual auto renderChar(char c) -> void = 0;
  virtual auto renderString(std::string_view s) -> void = 0;
  virtual auto moveToStart() -> void {
    SlowTrie S = SlowTrie();
    S.insert("aaa");
    wmove(wind_.get(),yOffset_,xOffset_);
  }
  virtual auto rebox() -> void {
    box(wind_.get(),vsep,hsep);
  }

  auto getWind() -> WINDOW* {
    return wind_.get();
  }

  // Decide how to render the window
  virtual auto render() -> void = 0;
  virtual auto refresh() -> void {
    wrefresh(wind_.get());
  };

  windowPtr<WINDOW> wind_;
  std::string textData_;
  int inity_;
  int initx_;
  int yOffset_;
  int xOffset_;
  char vsep = '|';
  char hsep = '-';
};

struct TextWindow : public SnickersWindow {

  TextWindow(int y, int x, int numRows, int numCols) : SnickersWindow(y,x,numRows,numCols) {};
  TextWindow(int y, int x, int numRows, int numCols, int yOffset, int xOffset) : SnickersWindow(y,x,numRows,numCols,yOffset,xOffset) {
    wrefresh(wind_.get());
    for(int i =0; i < COLS; i++)
        mvwprintw(wind_.get(),yOffset_,i," ");
    wrefresh(wind_.get());
    refresh();
    box(wind_.get(),vsep,hsep);
  };

  auto clear(bool mtStart) -> void {
    std::string tmp = "";
    for(int j = xOffset_; j < COLS - xOffset_; j++) {
        tmp += " ";
    }
    mvwprintw(wind_.get(),yOffset_,xOffset_,"%s",tmp.c_str());
    for(int i = 0; i < internal_count_; i++) {
        mvwprintw(wind_.get(),yOffset_+i,xOffset_,"%s",tmp.c_str());
    }
    internal_count_ = 0;
    textData_ = "";
    wrefresh(wind_.get());
    box(wind_.get(),vsep,hsep);
    if(mtStart)
        moveToStart();
  }

  auto renderChar(char c) -> void {
    box(wind_.get(),vsep,hsep);
    textData_+=c;
    mvwprintw(wind_.get(),yOffset_,xOffset_,"%s",textData_.c_str());
    wrefresh(wind_.get());
  }

  auto eraseChar() -> void {
    std::string tmp = "";
    for(int i =0; i < textData_.size();i++)
        tmp += " ";
    mvwprintw(wind_.get(),yOffset_,xOffset_,"%s",tmp.c_str());
    textData_ = textData_.substr(0,textData_.size()-1);
    mvwprintw(wind_.get(),yOffset_,xOffset_,"%s",textData_.c_str());
  }

  auto moveToStart() -> void {
    wmove(wind_.get(),yOffset_,xOffset_);
  }

  auto render() -> void {
    rebox();
    refresh();
  }

  auto getChar() -> char {
    return wgetch(wind_.get());
  }

  auto getString() -> std::string {
    return textData_;
  }
  auto renderString(std::string_view s) -> void {}

  auto renderNewString(std::string s) -> void {
    mvwprintw(wind_.get(),yOffset_+internal_count_,xOffset_,"%s",s.c_str());
    internal_count_++;
    box(wind_.get(),vsep,hsep);
    wrefresh(wind_.get());
  }

  private:
  int internal_count_ = 0;

};

struct SuggestionWindow : public SnickersWindow {
  SuggestionWindow(int y, int x, int numRows, int numCols, std::vector<std::string> suggestions) : SnickersWindow(y,x,numRows,numCols) {
    trie_.insert(suggestions);
  };
  SuggestionWindow(int y, int x, int numRows, int numCols, int yOffset, int xOffset, std::vector<std::string> suggestions) : SnickersWindow(y,x,numRows,numCols,yOffset,xOffset) {
    trie_.insert(suggestions);
  };

  auto renderChar(char c) -> void {}

  auto renderString(std::string_view s) -> void {
    auto results = trie_.getMatches(s);
    if(results.size() == 0)
      return;
    wbkgd(wind_.get(),COLOR_PAIR(9));
    werase(wind_.get());
    int size = results.size() < 2 ? 3 : 2 + results.size();
    int movesize = results.size() < 2 ? 6 : 5 + results.size();
    auto maxnum = std::max_element(results.begin(),results.end(),[](const auto&a, const auto& b) { return a.size() < b.size();});
    wind_.reset(newwin(size,maxnum->size()+4,LINES-movesize,0));
    wbkgd(wind_.get(),COLOR_PAIR(8));
    box(wind_.get(),0,0);
    int count = 0;
    for(auto& strings : results) {
        mvwprintw(wind_.get(),1+count,2,"%s",strings.c_str());
        count++;
    }
    wrefresh(wind_.get());
  }

  auto render() -> void {};

  auto clear(bool moveToStart) -> void {
    wbkgd(wind_.get(),COLOR_PAIR(9));
    wrefresh(wind_.get());
  }
  auto flip() -> void {
    wbkgd(wind_.get(),COLOR_PAIR(9));
    mvwprintw(wind_.get(),1,1,"%s","");
  }

  private:
  SlowTrie trie_;
  std::string data_;
};
