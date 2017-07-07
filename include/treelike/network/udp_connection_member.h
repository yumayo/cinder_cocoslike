#pragma once
#include <treelike/network/udp_connection.h>
#include <boost/asio.hpp>
using udp = boost::asio::ip::udp;
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <treelike/utility/scoped_mutex.h>
namespace treelike
{
namespace network
{
class udp_connection::member
{
    udp_connection& _connection;

    // UDPを使う上で必須の項目達。
    boost::asio::io_service _io_service;
    udp::socket _udp_socket;
    udp::endpoint _remote_endpoint;
    boost::array<char, 65536> _remote_buffer;

    // 非同期的に受信をしないとプログラムが止まってしまうので。
    std::thread _update_io_service;
    bool _is_pause = false;
    utility::recursion_usable_mutex _mutex;

    std::map<udp::endpoint, std::string> _receive_buffers;
private:
    member( udp_connection& server, udp::endpoint const& end_point );
public:
    member( ) = delete;
    member( udp_connection& server );
    member( udp_connection& server, int port_num );
    ~member( );

    void write( network_handle handle, Json::Value const& send_data );
    void write( network_handle handle, std::string const& send_data );
    void write( network_handle handle, char const* send_data );
    void write( network_handle handle, char const* send_data, size_t const& send_data_byte );

    void close( );
    void open( );

    int get_port( );

    void update( float delta_second );
private:
    // エントリーポイント
    void _receive( );
};
}

}