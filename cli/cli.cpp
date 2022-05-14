#include <stdlib.h>
#include <curses.h>
#include <string>
#include <signal.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "cli_tools.h"
#include "../cores/nes/6502.h"
#include "../assemblers/nes/nes.h"

static int quit_flag;
static void finish(int sig);

auto write_ram_map(CPU_6502& cpu, std::vector<std::byte>& bytes, std::string& asmOutput) -> size_t {
  size_t index = 0;
  for(auto& byte: bytes) {
    cpu.ram_->store(index,byte);
    asmOutput += std::to_string(std::to_integer<int>(byte)) + " ";
    index++;
  }
  return index;
}

int main(int argc, char *argv[])
{
    int num = 0;
    CPU_6502 bla{};
    NesAssembler assembler{};

    /* initialize your non-curses data structures here */

    (void) signal(SIGINT, finish);      /* arrange interrupts to terminate */

    (void) initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) cbreak();       /* take input chars one at a time, no wait for \n */
    (void) noecho();         /* echo input - in color */

    if (has_colors())
    {
        start_color();
        use_default_colors();

        /*
         * Simple color assignment, often all we need.  Color pair 0 cannot
         * be redefined.  This example uses the same value for the color
         * pair as for the foreground color, though of course that is not
         * necessary:
         */
        init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_CYAN,    COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
        init_pair(8, COLOR_BLACK,   COLOR_WHITE);
        init_pair(9, COLOR_BLACK,   COLOR_BLACK);
    }

    int y, x;
    TextWindow topWin2 = TextWindow(0,0,LINES-10,COLS-40,1,2);
    TextWindow cpuWin = TextWindow(0,COLS-40,LINES/2-11,40,1,2);
    TextWindow asmWin = TextWindow(LINES/2-11,COLS-40,LINES/2+2,40,1,2);
    TextWindow text2 = TextWindow(LINES-3,0,3,COLS,1,2);//TextWindow(3,COLS,LINES-3,0,2,1);
    SuggestionWindow wind2 = SuggestionWindow(LINES-10,0,3,10,0,0,bla.getInstructionStrings());//newwin(3,10,LINES-10,0);
    char c = '\0';
    text2.moveToStart();
    topWin2.refresh();
    cpuWin.renderString(bla.cliOutput());
    cpuWin.refresh();
    asmWin.refresh();
    for (;;)
    {
        c = text2.getChar();
        if(c == 13)
        {
            std::string cmdbuf = text2.getString();
            text2.clear(true);
            if(cmdbuf == "")
                continue;
            if(cmdbuf == "clear") {
                topWin2.clear(false);
            }
            else
                topWin2.renderNewString(cmdbuf);
            wind2.clear(false);
            topWin2.render();
            continue;
        } else if(c == 8 || c == 127) {
            wind2.clear(false);
            text2.eraseChar();
            bool hadSuggestion = false;
            if(text2.getString().size() != 0){
                hadSuggestion = wind2.renderString(text2.getString());
            }
            if(!hadSuggestion)
                topWin2.render();
            continue;
        } else if(c == 18) {
            bla.clear();
            auto instructions = topWin2.getBufferedStrings();
            std::string asmCode = "";
            for(auto& inst : instructions) {
                asmCode += inst + "\n";
            }
            std::transform(asmCode.begin(),asmCode.end(),asmCode.begin(),
                                     [](unsigned char c) {return std::toupper(c);});
            auto assembly = assembler.assemble(asmCode);
            std::string dec = "";
            auto until = write_ram_map(bla,assembly, dec);
            bla.runProgram(until);
            asmWin.renderString(dec);
            cpuWin.renderString(bla.cliOutput());
            continue;
        }
        wind2.flip();
        topWin2.rebox();
        wind2.refresh();
        topWin2.refresh();
        text2.renderChar(c);
        wind2.renderString(text2.getString());
        topWin2.refresh();
    }

    finish(0);               /* we are done */
}

static void finish(int sig)
{
    endwin();

    /* do your non-curses wrapup here */

    exit(0);
}
