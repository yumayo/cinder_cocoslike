#include <treelike/network/udp_connection.h>
#include <treelike/network/udp_connection_member.h>
namespace treelike
{
namespace network
{
CREATE_CPP( udp_connection )
{
    CREATE( udp_connection );
}
bool udp_connection::init( )
{
    set_schedule_update( );
    _m = std::make_shared<member>( *this );
    return true;
}
CREATE_CPP( udp_connection, int port )
{
    CREATE( udp_connection, port );
}
bool udp_connection::init( int port )
{
    set_schedule_update( );
    _m = std::make_shared<member>( *this, port );
    return true;
}
void udp_connection::write( network_handle handle, Json::Value const & send_data )
{
    _m->write( handle, send_data );
}
void udp_connection::write( network_handle handle, std::string const & send_data )
{
    _m->write( handle, send_data );
}
void udp_connection::write( network_handle handle, char const * send_data )
{
    _m->write( handle, send_data );
}
void udp_connection::write( network_handle handle, char const * send_data, size_t send_data_byte )
{
    _m->write( handle, send_data, send_data_byte );
}
int udp_connection::get_port( )
{
    return _m->get_port( );
}
void udp_connection::close( )
{
    _m->close( );
}
void udp_connection::open( )
{
    _m->open( );
}
void udp_connection::update( float delta_second )
{
    _m->update( delta_second );
}
void udp_connection::on( std::string const& name, std::function<void( network_handle const&, Json::Value )> const& func )
{
    on_received_named_json.insert( std::make_pair( name, func ) );
}
}
}