#include "../core/rom.h"
#include <iostream>

int main()
{
	Rom rom( "./Ladida_lol.sfc");	
	std::cout << rom.size() << "\n";
	return 0;
}
