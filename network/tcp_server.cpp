#include "tcp_server.h"
#include "tcp_server_member.h"
namespace network
{
CREATE_CPP( tcp_server, std::string const& port, int num_of_client )
{
    CREATE( tcp_server, port, num_of_client );
}
tcp_server::~tcp_server( )
{
    _m->io.stop( );
}
bool tcp_server::init( std::string const& port, int num_of_client )
{
    _m.reset( );
    _m = std::make_shared<_member>( *this, port, num_of_client );

    set_schedule_update( );

    for ( int i = 0; i < num_of_client + 1; ++i )
    {
        _m->async_accept( *_m->sockets[i] );
    }

    return true;
}
void tcp_server::update( float delta )
{
    _m->io.reset( );
    _m->io.poll( );
}
void tcp_server::write( client_handle const& handle, std::string const & message, std::function<void( )> on_send )
{
    write( handle, message.c_str( ), message.size( ), on_send );
}
void tcp_server::write( client_handle const& handle, char const * message, size_t size, std::function<void( )> on_send )
{
    _m->find_run( handle, [ this, message, size, on_send ] ( socket_object& sock_obj )
    {
        _m->write( sock_obj, asio::buffer( message, size ), on_send );
    } );
}
void tcp_server::speech( std::string const & message, std::function<void( )> on_send )
{
    speech( message.c_str( ), message.size( ), on_send );
}
void tcp_server::speech( char const * message, size_t size, std::function<void( )> on_send )
{
    for ( auto& obj : _m->sockets )
    {
        if ( !obj->handle.ip_address.empty( ) &&
             !obj->handle.port.empty( ) )
        {
            _m->write( *obj, asio::buffer( message, size ), on_send );
        }
    }
}
void tcp_server::close( client_handle const& handle )
{
    _m->find_run( handle, [ this ] ( socket_object& sock_obj )
    {
        _m->close_with_async( sock_obj );
    } );
}
std::vector<client_handle> tcp_server::get_clients( )
{
    std::vector<client_handle> clients;
    for ( auto& obj : _m->sockets )
    {
        if ( !obj->handle.ip_address.empty( ) &&
             !obj->handle.port.empty( ) )
        {
            clients.emplace_back( obj->handle );
        }
    }
    return clients;
}
}