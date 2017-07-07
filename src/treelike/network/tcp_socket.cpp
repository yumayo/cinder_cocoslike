#include <treelike/network/tcp_socket.h>
namespace treelike
{
namespace network
{
tcp_socket::tcp_socket( boost::asio::io_service& io )
    : socket( io )
    , handle( _ip_address, _port )
{
    buffer.fill( 0 );
}
void tcp_socket::close( )
{
    _ip_address = "";
    _port = 0;
    socket.close( );
    buffer.fill( 0 );
}
void tcp_socket::set_client_handle( std::string const& ip_address, int port )
{
    _ip_address = ip_address;
    _port = port;
}
void tcp_socket::buffer_clear( size_t bytes_transferred )
{
    std::fill_n( buffer.begin( ), bytes_transferred, 0 );
}
char const* tcp_socket::c_str( )
{
    return buffer.data( );
}
}
}