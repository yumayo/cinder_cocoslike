#pragma once
#include "tcp_client.h"
#ifndef ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_STD_ATOMIC
#endif
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
#include "boost/array.hpp"
namespace network
{
struct tcp_client::_member
{
    _member( tcp_client& parent, std::string const& ip_address, std::string const& port )
        : parent( parent )
        , io( )
        , socket( io, asio::ip::tcp::endpoint( asio::ip::tcp::v4( ), 0 ) )
        , ip_address( ip_address )
        , port( port )
    {
        buffer.fill( 0 );
    }
    tcp_client& parent;
    void connect( );
    void write( asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void read( );
    void close( );
    void error( asio::error_code const& error );
    int get_port( );
    asio::io_service io;
    asio::ip::tcp::socket socket;
    boost::array<char, 1024 * 256> buffer;
    std::string ip_address;
    std::string port;
};
}