#pragma once
#include <jsoncpp/json.h>
#include <functional>
#include <treelike/network/network_handle.h>
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
    CREATE_H( udp_connection, int port );
    bool init( int port );
    void write( network_handle handle, Json::Value const& send_data );
    void write( network_handle handle, std::string const& send_data );
    void write( network_handle handle, char const* send_data );
    void write( network_handle handle, char const* send_data, size_t send_data_byte );
    int get_port( );
    void close( );
    void open( );
    void on( std::string const& name, std::function<void( network_handle const&, Json::Value )> const& func );
public:
    void update( float delta_second ) override;
public:
    std::function<void( network_handle handle, char const* received_data, size_t received_data_byte )> on_received;
    std::function<void( network_handle handle, Json::Value root )> on_received_json;
    std::map<std::string, std::function<void( network_handle handle, Json::Value root )>> on_received_named_json;
    std::function<void( )> on_receive_failed;
    std::function<void( )> on_sended;
    std::function<void( )> on_send_failed;
};
}

}