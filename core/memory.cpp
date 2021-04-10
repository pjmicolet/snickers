#include "memory.h"

RAM::RAM( size_t ramSize, int banks ) noexcept : banks_(banks) {
    ram_.reserve( banks );
    int ramLines = ramSize/banks;
    this->lines_ = ramLines;
    for( int i = 0; i < banks; i++ )
        ram_.emplace_back();
    for( auto& ramBank : ram_ )
    {
        ramBank.reserve( ramLines );
        for( int i = 0; i < ramLines; i++ )
            ramBank.emplace_back(i);    
    }
}

RAM::RAM( size_t ramSize, std::vector<int>& tracedLines, int banks ) noexcept : banks_(banks) {
    ram_.reserve( banks );
    int ramLines = ramSize/banks;
    int tracing = 0;
    int lines = 0;
    this->lines_ = ramLines;

    for( int i = 0; i < banks; i++ )
        ram_.emplace_back();

    for( auto& ramBank : ram_ ) {
        ramBank.reserve( ramLines );
        for( int i = 0; i < ramLines; i++,lines++ ) {
            bool trace = (int)lines == tracedLines[ tracing ];
            if( trace ) tracing++;
            ramBank.emplace_back(i,trace);    
        }
    }
}

auto RAM::validateRAM( bankIndexPair& pair ) -> void {
    bool bankIssue = pair.first > ( banks_ - 1 );
    bool lineIssue = pair.second > ( lines_ - 1 );

    if( bankIssue || lineIssue ) {
        std::string throwMsg = "Trying to access invalid RAM line/bank! Bank["+std::to_string( pair.first )+"]:Line["+std::to_string( pair.second )+"] only have " + std::to_string( banks_ ) + " banks and " + std::to_string( lines_ ) + " lines per bank!";
        throw RamException( throwMsg );
    }
}

// For SNES we have something like 0xBBIIII
// where BB is the bank number and IIII is the index of the bank we're accessing
#if DEBUG
auto RAM::calculateBank( int index ) -> bankIndexPair {
#else
auto RAM::calculateBank( int index ) noexcept -> bankIndexPair {
#endif
    auto bank = ( index & 0xFF0000 ) >> 16;
    auto subindex = ( index & 0xFFFF );
    auto bankIndex = std::make_pair( bank, subindex );
#if DEBUG
    validateRAM( bankIndex );
#endif
    return bankIndex; 
}

#if DEBUG
auto RAM::store( int index, std::byte data ) -> void {
#else
auto RAM::store( int index, std::byte data ) noexcept -> void {
#endif
    auto bankIndex = calculateBank( index );
    ram_[bankIndex.first][ bankIndex.second ] = data;
}
