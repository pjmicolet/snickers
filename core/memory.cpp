#include "memory.h"

RAM::RAM( size_t ramSize ) {
	ram_.resize( ramSize );
}

RAM::RAM( size_t ramSize, std::vector<int>& tracedLines ) {
	ram_.resize( ramSize );
	for( auto& tracedLine : tracedLines ) {
		ram_[ tracedLine ].traced_ = true;
	}
}

auto RAM::store( int index, uint16_t data ) -> void {
	uint16_t before = 0;
	auto log = ram_[ index ].traced_;
	if( log )
		std::cout << "RAM[" << std::hex << index << "] = 0x"<< ram_[ index ].data_ << " -> 0x" << data << "\n";
	ram_[ index ].data_ = data;
}
