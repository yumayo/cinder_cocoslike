#include "udp_connection.h"
#include "udp_connection_member.h"
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
CREATE_CPP( udp_connection, int const & port_number )
{
    CREATE( udp_connection, port_number );
}
bool udp_connection::init( int const & port_number )
{
    set_schedule_update( );
    _m = std::make_shared<member>( *this, port_number );
    return true;
}
void udp_connection::write( network_handle const & handle, Json::Value const & send_data )
{
    _m->write( handle, send_data );
}
void udp_connection::write( network_handle const & handle, std::string const & send_data )
{
    _m->write( handle, send_data );
}
void udp_connection::write( network_handle const & handle, char const * send_data )
{
    _m->write( handle, send_data );
}
void udp_connection::write( network_handle const & handle, char const * send_data, size_t send_data_byte )
{
    _m->write( handle, send_data, send_data_byte );
}
std::list<std::shared_ptr<network_object>>& udp_connection::get_clients( )
{
    return _m->get_clients( );
}
utility::recursion_usable_mutex & udp_connection::get_mutex( )
{
    return _m->get_mutex( );
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
}
