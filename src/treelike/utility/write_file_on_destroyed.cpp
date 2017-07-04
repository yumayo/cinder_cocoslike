#include <treelike/utility/write_file_on_destroyed.h>
#include <treelike/utility/string.h>
#include <stdarg.h>
#include <fstream>
namespace treelike
{
namespace utility
{
write_file_on_destroyed::write_file_on_destroyed( std::string const & filename )
    : _filename( filename )
{
}
write_file_on_destroyed::~write_file_on_destroyed( )
{
    std::ofstream output( cinder::app::getWritablePath( _filename ) );
    output << *this;
}
void write_file_on_destroyed::operator()( char const * str, ... )
{
    const int max_string_length = ( 1024 * 100 );
    std::string formated_string;
    va_list args;
    va_start( args, str );
    char buf[max_string_length];
    vsnprintf( buf, max_string_length, str, args );
    formated_string = buf;
    va_end( args );
    *this += formated_string;
}
}

}