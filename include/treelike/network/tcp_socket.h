#pragma once
#include <boost/asio.hpp>
#include <treelike/network/network_handle.h>
#include <array>
namespace treelike
{
namespace network
{
class tcp_socket
{
public:
    tcp_socket( ) = delete;
    tcp_socket( tcp_socket const& ) = delete;
    tcp_socket( boost::asio::io_service& io );
    void close( );
    void set_client_handle( std::string const& ip_address, int port );
    void buffer_clear( size_t bytes_transferred );
    char const* c_str( );
    boost::asio::ip::tcp::socket socket;
    std::array<char, 65536> buffer;
    std::string stack;
    network_handle handle;
};
}
}