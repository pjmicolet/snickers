#include "memory.h"

RAM::RAM( size_t ramSize ) {
	ram_.reserve( ramSize );
	for( int i = 0; i < ramSize; i++ )
		ram_.emplace_back(i);	
}

RAM::RAM( size_t ramSize, std::vector<int>& tracedLines ) {
	ram_.reserve( ramSize );
	int tracing = 0;
	for( int i = 0; i < ramSize; i++ ) {
		bool trace = (int)i == tracedLines[ tracing ];
		if( trace ) tracing++;
		ram_.emplace_back(i,trace);	
	}
}

auto RAM::store( int index, uint16_t data ) -> void {
	uint16_t before = 0;
	ram_[ index ] = data;
}
