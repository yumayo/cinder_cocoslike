#include <treelike/utility/time_stamp.h>
#include <chrono>
using namespace std::chrono;
namespace treelike
{
namespace utility
{
double time_stamp::get_time_second( )
{
    return static_cast<double>( duration_cast<nanoseconds>( steady_clock::now( ).time_since_epoch( ) ).count( ) ) / 1000000000;
}
double time_stamp::start( )
{
    return _start_second = get_time_second( );
}
double time_stamp::end( )
{
    return _end_second = get_time_second( );
}
double time_stamp::get_start_second( )
{
    return _start_second;
}
double time_stamp::get_end_second( )
{
    return _end_second;
}
double time_stamp::get_elapsed_second( )
{
    return _end_second - _start_second;
}
}
}
