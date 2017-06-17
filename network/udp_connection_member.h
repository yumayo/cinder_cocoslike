#pragma once
#include "udp_connection.h"

// boost/Asioを使うためのマクロ4つ分。
// https://boostjp.github.io/tips/build_link.html
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
#define BOOST_ERROR_CODE_HEADER_ONLY
#define BOOST_SYSTEM_NO_LIB
#include <asio/asio.hpp>
using udp = asio::ip::udp;

#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

#include "network_factory.h"

namespace network
{
class udp_connection::member
{
    udp_connection& _connection;

    // UDPを使う上で必須の項目達。
    asio::io_service _io_service;
    udp::socket _udp_socket;
    udp::endpoint _remote_endpoint;
    boost::array<char, 1024 * 256> _remote_buffer;

    // 非同期的に受信をしないとプログラムが止まってしまうので。
    std::thread _update_io_service;
    bool _is_pause = false;
    utility::recursion_usable_mutex _mutex;

    // 繋がったオブジェクトたちを保存しておきます。
    network_factory _network_factory;

    std::deque<std::pair<udp::endpoint, std::vector<char>>> _receive_deque;
private:
    member( udp_connection& server, udp::endpoint const& end_point );
public:
    member( ) = delete;
    member( udp_connection& server );
    member( udp_connection& server, int const& port_num );
    ~member( );

    void write( network_handle const& handle, Json::Value const& send_data );
    void write( network_handle const& handle, std::string const& send_data );
    void write( network_handle const& handle, char const* send_data );
    void write( network_handle const& handle, char const* send_data, size_t const& send_data_byte );

    void close( );
    void open( );

    bool destroy_client( network_handle const& handle);
    network_handle regist_client( std::string const& ip_address, int const& port );
    std::list<std::shared_ptr<network_object>>& get_clients( );

    int get_port( );

    void update( float delta_second );
private:
    // エントリーポイント
    void _receive( );
};
}
