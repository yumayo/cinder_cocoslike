#pragma once
#include "tcp_server.h"
#ifndef ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_STD_ATOMIC
#endif
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
#include "client_handle.h"
#include "boost/array.hpp"
namespace network
{
class socket_object
{
public:
    socket_object( ) = delete;
    socket_object( socket_object const& ) = delete;
    socket_object( asio::io_service& io )
        : socket( io )
        , handle( _ip_address, _port )
    {
        buffer.fill( 0 );
    }
    void close( )
    {
        _ip_address = "";
        _port = "";
        socket.close( );
        buffer.fill( 0 );
    }
    void connect( std::string const& ip_address, std::string const& port )
    {
        _ip_address = ip_address;
        _port = port;
    }
    void buffer_clear( size_t bytes_transferred )
    {
        std::fill_n( buffer.begin( ), bytes_transferred, 0 );
    }
    char const* c_str( )
    {
        return buffer.data( );
    }
    asio::ip::tcp::socket socket;
    boost::array<char, 1024 * 256> buffer;
    client_handle handle;
private: // 以下の値をハンドルに詰め込んで運びます。
    std::string _ip_address;
    std::string _port;
};
struct tcp_server::_member
{
    _member( tcp_server& parent, std::string const& port, int num_of_client );
    void async_accept( socket_object& sock_obj );
    bool is_max( );
    void write( socket_object& sock_obj, asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void read( socket_object& sock_obj );
    void on_errored( socket_object& sock_obj, asio::error_code const& e );
    void close_with_async( socket_object& sock_obj );
    void find_run( client_handle const& handle, std::function<void( socket_object& )> call );
    tcp_server& parent;
    asio::io_service io;
    std::unique_ptr<asio::ip::tcp::acceptor> acceptor;
    std::vector<std::unique_ptr<socket_object>> sockets;
    std::string port;
};
}