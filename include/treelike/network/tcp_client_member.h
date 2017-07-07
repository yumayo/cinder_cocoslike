#pragma once
#include <treelike/network/tcp_client.h>
#include <boost/asio.hpp>
#include <array>
namespace treelike
{
namespace network
{
struct tcp_client::_member
{
    _member( tcp_client& parent, std::string const& ip_address, int port )
        : parent( parent )
        , io( )
        , socket( io, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4( ), 0 ) )
        , ip_address( ip_address )
        , port( port )
    {
        buffer.fill( 0 );
    }
    tcp_client& parent;
    void connect( );
    void write( char const* begin, size_t byte, std::function<void( )> on_send );
    void update( );
    void read( );
    void close( );
    void error( boost::system::error_code const& error );
    int get_port( );
    boost::asio::io_service io;
    boost::asio::ip::tcp::socket socket;
    std::array<char, 65536> buffer;
    std::string receive_buffer;
    std::string ip_address;
    int port;
};
}
}