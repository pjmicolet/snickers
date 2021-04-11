#include "memory.h"

RAM::RAM( size_t ramSize, int banks, Mirrors mirrors ) noexcept : banks_(banks) {
    ram_.reserve( banks );
    int ramLines = ramSize/banks;
    int8_t bank = 0;
    this->lines_ = ramLines;

    for( int i = 0; i < banks; i++ )
    {
        if( mirrors.contains( i ) )
            ram_.emplace_back( (size_t) ramLines, i, mirrors[i] );
        else
            ram_.emplace_back( (size_t) ramLines, i );
    }

    for( auto& ramBank : ram_ ) {
        for( int i = 0; i < ramLines; i++ )
            ramBank.populate(i);    
        bank++;
    }
}

RAM::RAM( size_t ramSize, std::vector<int>& tracedLines, int banks ) noexcept : banks_(banks) {
    ram_.reserve( banks );
    int ramLines = ramSize/banks;
    int tracing = 0;
    int lines = 0;
    this->lines_ = ramLines;
    int8_t bank = 0;

    for( int i = 0; i < banks; i++ )
        ram_.emplace_back((size_t) ramLines, i );

    for( auto& ramBank : ram_ ) {
        for( int i = 0; i < ramLines; i++,lines++ ) {
            bool trace = (int)lines == tracedLines[ tracing ];
            if( trace ) tracing++;
            ramBank.populate(i,trace);    
        }
        bank++;
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
    auto mirrorBanks = ram_[bankIndex.first].bankMirrors_;
    if( !mirrorBanks.empty() ) {
        for( auto& bank : mirrorBanks )
            ram_[bank][ bankIndex.second ] = data;
    }

    ram_[bankIndex.first][ bankIndex.second ] = data;
}
