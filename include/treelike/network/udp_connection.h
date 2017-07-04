#pragma once
#include <treelike/network/network_object.h>
#include <jsoncpp/json.h>
#include <functional>
#include <cinder/gl/scoped.h>
#include <treelike/utility/scoped_mutex.h>
#include <treelike/node.h>
namespace treelike
{
namespace network
{
class udp_connection : public node
{
    class member;
    hardptr<member> _m;
public:
    CREATE_H( udp_connection );
    bool init( );
    CREATE_H( udp_connection, int const& port_number );
    bool init( int const& port_number );
    void write( network_handle const& handle, Json::Value const& send_data );
    void write( network_handle const& handle, std::string const& send_data );
    void write( network_handle const& handle, char const* send_data );
    void write( network_handle const& handle, char const* send_data, size_t send_data_byte );
    bool destroy_client( network_handle const& handle );
    network_handle regist_client( std::string const& ip_address, int const& port );
    std::list<hardptr<network_object>>& get_clients( );
    int get_port( );
    void close( );
    void open( );
public:
    void update( float delta_second ) override;
public:
    // receive�֌W�͕ʃX���b�h�ł̌Ăяo���Ȃ̂�network_handle�������ۂɁAmutex��t���ĕϐ��𑀍삵�Ă��������B

    std::function<void( network_handle handle, 
                        char const* received_data, 
                        size_t received_data_byte )> on_received;
    std::function<void( network_handle handle,
                        Json::Value root )> on_received_json;
    std::map<std::string, std::function<void( network_handle handle,
                                              Json::Value root )>> on_received_named_json;
    std::function<void( )> on_receive_failed;

    std::function<void( )> on_sended;
    std::function<void( )> on_send_failed;
};
}

}