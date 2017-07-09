#include <treelike/network/tcp_server.h>
#include <treelike/network/tcp_server_member.h>
namespace treelike
{
namespace network
{
CREATE_CPP( tcp_server, int port, int num_of_client )
{
    CREATE( tcp_server, port, num_of_client );
}
tcp_server::~tcp_server( )
{
    _m->io.stop( );
}
bool tcp_server::init( int port, int num_of_client )
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
    _m->update( );
}
void tcp_server::write( network_handle const& handle, std::string const & message, std::function<void( )> on_send )
{
    write( handle, message.c_str( ), message.size( ), on_send );
}
void tcp_server::write( network_handle const& handle, char const * message, size_t size, std::function<void( )> on_send )
{
    _m->find_run( handle, [ this, message, size, on_send ] ( tcp_socket& sock_obj )
    {
        _m->write( sock_obj, message, size, on_send );
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
        if ( obj->handle )
        {
            _m->write( *obj, message, size, on_send );
        }
    }
}
void tcp_server::close( network_handle const& handle )
{
    _m->find_run( handle, [ this ] ( tcp_socket& sock_obj )
    {
        _m->close_with_async( sock_obj );
    } );
}
std::vector<network_handle> tcp_server::get_clients( )
{
    std::vector<network_handle> clients;
    for ( auto& obj : _m->sockets )
    {
        if ( obj->handle )
        {
            clients.emplace_back( obj->handle );
        }
    }
    return clients;
}
void tcp_server::on( std::string const& name, std::function<void( network_handle const&, Json::Value )> const& func )
{
    on_received_named_json.insert( std::make_pair( name, func ) );
}
}
}