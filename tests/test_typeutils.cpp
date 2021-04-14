#include <iostream>
#include "test_utils.h"
#include "../utils/type_utils.h"

auto test_type_basic() -> bool {
    uint8_t singleByte = 120;
    auto byteArray = integerToByteV<uint8_t>( singleByte );

    if( byteArray.size() != 1 )
        return false;

    byteArray = integerToByteV<uint16_t>( singleByte );
    if( byteArray.size() != 2 )
        return false;

    byteArray = integerToByteVTrim<uint16_t>( singleByte );
    if( byteArray.size() != 1 )
        return false;

    auto intVer = byteVecToInteger<uint8_t>( byteArray );
    
    if( intVer != 120 )
        return false;

    auto intVer2 = byteVecToInteger<uint16_t>( byteArray );
    if (intVer2 != 120 )
        return false;

    uint16_t maxBits16 = 0xFFFF;
    byteArray = integerToByteV<uint16_t>( maxBits16 );
    if ( byteArray[0] != std::byte{0xFF} && byteArray[1] != std::byte{0xFF} )
        return false;

    auto small = byteVecToInteger<uint8_t>( byteArray );
    if( small != 0xFF )
        return false;
    return true;
}

auto test_typeutils() -> bool {
    bool passed = true;
    RUN_TEST( test_type_basic() );
    return passed;
}
