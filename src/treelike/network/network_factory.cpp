#include <treelike/network/network_factory.h>
#include <treelike/utility/string_utility.h>
namespace treelike
{
namespace network
{
network_factory::network_factory( udp_connection & connection )
    : _connection( connection )
{
}
network_handle network_factory::regist( std::string const & ip_address, int const & port )
{
    auto regist_object = std::make_shared<network_object>( ip_address, port );
    auto itr = find_network_object( regist_object );
    if ( itr == std::end( _network_objects ) )
    {
        _network_objects.emplace_back( regist_object );
        return regist_object;
    }
    else
    {
        return ( *itr );
    }
}
bool network_factory::destroy_client( network_handle handle )
{
    if ( _network_objects.empty( ) ) return false;
    auto remove_itr = std::remove_if( std::begin( _network_objects ), std::end( _network_objects ),
                                      [ handle ] ( hardptr<network_object>& objects )
    {
        if ( objects == handle )
        {
            utility::log_network( objects->ip_address, objects->port,
                                  "このオブジェクトは削除されました。" );
        }
        return objects == handle;
    } );
    return _network_objects.erase( remove_itr, std::end( _network_objects ) ) != std::end( _network_objects );
}
std::list<hardptr<network_object>>::iterator network_factory::find_network_object( network_handle handle )
{
    auto itr = std::find_if( std::begin( _network_objects ), std::end( _network_objects ),
                             [ handle ] ( hardptr<network_object>& object )
    {
        return object == handle;
    } );
    if ( itr != std::end( _network_objects ) )
    {
        return itr;
    }
    return std::end( _network_objects );
}
std::list<hardptr<network_object>>& network_factory::get_clients( )
{
    return _network_objects;
}
void network_factory::update( float delta_second )
{
    auto remove_itr = std::remove_if( std::begin( _network_objects ), std::end( _network_objects ),
                                      [ delta_second ] ( hardptr<network_object>& objects )
    {
        objects->update( delta_second );
        if ( objects->is_destroy( ) )
        {
            utility::log_network( objects->ip_address, objects->port,
                                  "タイムアウトになりました。このオブジェクトは削除されました。" );
            return true;
        }
        return false;
    } );
    _network_objects.erase( remove_itr, std::end( _network_objects ) );
}
}

}