#pragma once
#include "udp_connection.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include "network_factory.h"

namespace network
{
class udp_connection::member
{
    using udp = boost::asio::ip::udp;

    udp_connection& _connection;

    // UDP���g����ŕK�{�̍��ڒB�B
    boost::asio::io_service _io_service;
    udp::socket _udp_socket;
    udp::endpoint _remote_endpoint;
    boost::array<char, 2048> _remote_buffer;

    // �񓯊��I�Ɏ�M�����Ȃ��ƃv���O�������~�܂��Ă��܂��̂ŁB
    std::thread _update_io_service;
    bool _is_pause = false;
    utility::recursion_usable_mutex _mutex;

    // �q�������I�u�W�F�N�g������ۑ����Ă����܂��B
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

    std::list<std::shared_ptr<network_object>>& get_clients( );

    void update( float delta_second );
private:
    // �G���g���[�|�C���g
    void _receive( );
};
}
