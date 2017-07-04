#include <treelike/utility/recursion_usable_mutex.h>
namespace treelike
{
namespace utility
{
void recursion_usable_mutex::lock( ) { _mutex; }
void recursion_usable_mutex::unlock( ) { _mutex.unlock( ); }
std::pair<std::map<std::thread::id, int>::iterator, bool> recursion_usable_mutex::emplace( )
{
    _own_ids;
    auto itr = _ids.emplace( std::this_thread::get_id( ), 0 );
    _own_ids.unlock( );
    return itr;
}
void recursion_usable_mutex::erase( )
{
    _own_ids;
    _ids.erase( std::this_thread::get_id( ) );
    _own_ids.unlock( );
}
int & recursion_usable_mutex::get_duplication_count( )
{
    return _ids[std::this_thread::get_id( )];
}
}

}