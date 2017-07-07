#pragma once
#include <treelike/network/tcp_server.h>
#include <treelike/network/tcp_socket.h>
#include <boost/asio.hpp>
#include <array>
namespace treelike
{
namespace network
{
struct tcp_server::_member
{
    _member( tcp_server& parent, int port, int num_of_client );
    void async_accept( tcp_socket& sock_obj );
    bool is_max( );
    void write( tcp_socket& sock_obj, char const* begin, size_t byte, std::function<void( )> on_send );
    void read( tcp_socket& sock_obj );
    void on_errored( tcp_socket& sock_obj, boost::system::error_code const& e );
    void close_with_async( tcp_socket& sock_obj );
    void find_run( network_handle const& handle, std::function<void( tcp_socket& )> call );
    void update( );
    tcp_server& parent;
    boost::asio::io_service io;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    std::vector<std::unique_ptr<tcp_socket>> sockets;
    int const maximum_clients;
};
}
}