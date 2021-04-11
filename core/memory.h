#include <cstdint>
#include <vector>
#include <iostream>
#include <cstddef>
#include <utility>
#include <unordered_map>

class RamException : public std::exception {
public:
    RamException( std::string message ) : exception_msg_( message ) {};
    virtual const char* what() const throw() {
        return exception_msg_.c_str();
    }    
private:
    std::string exception_msg_;
};

class RAM {
public:
    using Mirrors = std::unordered_map< int, std::vector< int > >;
    // Don't want a default constructor because it means very little
    RAM() = delete;
    RAM( size_t ramSize, int banks = 1, Mirrors mirrors = {} ) noexcept;
    RAM( size_t ramSize, std::vector<int>& tracedLines, int banks = 1 ) noexcept;
    RAM( size_t ramSize, std::vector<int>& tracedLines, int banks = 1, Mirrors mirrors = {} ) noexcept;

    struct RamLine {
        RamLine(int index) : traced_(false), data_( std::byte{0} ), index_( index ) {};
        RamLine(int index, int8_t bank) : traced_(false), data_( std::byte{0} ), index_( index ), bank_( bank ) {};
        RamLine(int index, bool traced) : traced_(traced), data_( std::byte{0} ), index_( index ) {};
        RamLine(int index, int8_t bank, bool traced) : traced_(traced), data_( std::byte{0} ), index_( index ), bank_( bank ) {};

        friend auto operator<< (std::ostream& os, const RamLine& data ) -> std::ostream& {
            return os << "RAM[" << std::hex << data.index_ << "]";
        }

        auto operator= ( std::byte data ) -> RamLine& {
            if( traced_ )
                std::cout << this << std::hex << " = 0x"<< std::to_integer<uint16_t>( data_ ) << " -> 0x" << std::to_integer<uint16_t>( data ) << "\n";
            data_ = data;
            return *this;
        }

        auto operator= ( RamLine& line ) -> RamLine& {
            if( traced_ )
                std::cout << this << std::hex << " = 0x"<< std::to_integer<uint16_t>( data_ ) << " -> 0x" << std::to_integer<uint16_t>( line.data_ ) << "\n";
            data_ = line.data_;
            return *this;
        }

        auto operator== ( std::byte data ) const -> bool {
            return data_ == data;
        }

        auto operator== ( const RamLine& line ) const -> bool {
            return data_ == line.data_;
        }

        int8_t bank_;
        protected:
            std::byte data_;

        private:
            bool traced_;
            int index_;
    };

    struct Bank {
        Bank( size_t numLines, int bankNum ) : bankNum_( bankNum ) { lines_.reserve(numLines); };
        Bank( size_t numLines, int bankNum, std::vector<int> bankMirrors ) : bankMirrors_( bankMirrors ), bankNum_( bankNum ) { lines_.reserve( numLines ); }; // std::move ?
        
        inline void populate( int index, bool traced = false ) {
           lines_.emplace_back( index, bankNum_, traced );
        }   
        auto operator[]( int index ) -> RamLine& {
            return lines_[index];
        }

        std::vector< int > bankMirrors_;
        private:
            std::vector< RamLine > lines_;
            int bankNum_;
    };

#if DEBUG
    auto operator[]( int index ) -> RamLine& {
#else
    auto operator[]( int index ) noexcept -> RamLine& {
#endif
        auto bankIndex = calculateBank( index );
        return ram_[bankIndex.first][ bankIndex.second ];
    }

#if DEBUG
    auto store( int index, std::byte data ) -> void;
#else
    auto store( int index, std::byte data ) noexcept -> void;
#endif


private:
    using bankIndexPair = std::pair<int,int>;
#if DEBUG
    auto calculateBank( int index ) -> bankIndexPair;
#else
    auto calculateBank( int index ) noexcept -> bankIndexPair;
#endif
    std::vector< Bank > ram_;
    auto validateRAM( bankIndexPair& pair ) -> void;
    int banks_;
    int lines_;
};
