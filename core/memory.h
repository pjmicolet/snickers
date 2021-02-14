#include <cstdint>
#include <vector>
#include <iostream>

class RAM {
public:
    RAM() = delete;
    RAM( size_t ramSize );
    RAM( size_t ramSize, std::vector<int>& tracedLines );

    struct RamLine {
        RamLine(int index) : traced_(false), data_(0), index_( index ) {};
        RamLine(int index, bool traced) : traced_(traced), data_(0), index_( index ) {};

        friend auto operator<< (std::ostream& os, const RamLine& data ) -> std::ostream& {
            return os << "RAM[" << std::hex << data.index_ << "]";
        }

        auto operator= ( uint16_t data ) -> RamLine& {
            if( traced_ )
                std::cout << this << std::hex << " = 0x"<< data_ << " -> 0x" << data << "\n";
            data_ = data;
            return *this;
        }

        auto operator= ( RamLine& line ) -> RamLine& {
            if( traced_ )
                std::cout << this << std::hex << " = 0x"<< data_ << " -> 0x" << line.data_ << "\n";
            data_ = line.data_;
            return *this;
        }

        auto operator== ( uint16_t data ) -> bool {
            return data_ == data;
        }

        auto operator!= ( uint16_t data ) -> bool {
            return data_ != data;
        }
    
        auto operator== ( const RamLine& line ) -> bool {
            return data_ == line.data_;
        }

        auto operator!= ( const RamLine& line ) -> bool {
            return data_ != line.data_;
        }

        uint16_t data_;

        private:
            bool traced_;
            int index_;
    };

    auto operator[]( int index ) -> RamLine& {
        return ram_[ index ];
    }

    auto store( int index, uint16_t data ) -> void;

private:
    std::vector< RamLine > ram_;
        
};
