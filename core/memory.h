#include <cstdint>
#include <vector>
#include <iostream>

class RAM {
public:
	RAM() = delete;
	RAM( size_t ramSize );
	RAM( size_t ramSize, std::vector<int>& tracedLines );
	struct RamLine {
		RamLine() : traced_(false), data_(0) {};
		bool traced_;
		uint16_t data_;
	};

	auto operator[]( int index ) -> uint16_t {
		if( ram_[ index ].traced_ )
			std::cout << "RAM[" << std::hex << index << "] = 0x"<< ram_[ index ].data_ << "\n";
		return ram_[ index ].data_;
	}

	auto store( int index, uint16_t data ) -> void;

private:
	std::vector< RamLine > ram_;
		
};
