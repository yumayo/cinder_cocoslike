// https://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
#include "base64.h"
#include <sstream>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
namespace utility
{
using namespace boost::archive::iterators;
typedef base64_from_binary <transform_width<const char *, 6, 8> > base64_enc;
typedef transform_width<binary_from_base64<const char *>, 8, 6> base64_dec;
static std::string const base64_padding [ ] = { "", "==","=" };
std::string base64_encode( char const * begin, size_t byte )
{
    std::stringstream os;
    std::copy( base64_enc( begin ), base64_enc( begin + byte ),
               std::ostream_iterator<char>( os ) );

    os << base64_padding[byte % 3];
    return os.str( );
}
std::pair<std::unique_ptr<char [ ]>, size_t> base64_decode( std::string const & base64 )
{
    size_t string_byte = base64.size( );

    if ( string_byte && base64[string_byte - 1] == '=' )
    {
        --string_byte;
        if ( string_byte && base64[string_byte - 1] == '=' ) --string_byte;
    }
    if ( string_byte == 0 ) return std::pair<std::unique_ptr<char [ ]>, size_t>( );

    size_t binary_byte = string_byte * ( 6.0F / 8.0F );

    std::unique_ptr<char [ ]> binary( new char[binary_byte] );

    std::copy( base64_dec( base64.data( ) ), base64_dec( base64.data( ) + string_byte ), binary.get( ) );

    return std::make_pair( std::move( binary ), binary_byte );
}
}
