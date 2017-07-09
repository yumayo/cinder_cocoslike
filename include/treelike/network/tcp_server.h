#pragma once
#include <treelike/node.h>
#include <boost/system/error_code.hpp>
#include <treelike/network/network_handle.h>
#include <jsoncpp/json.h>
namespace treelike
{
namespace network
{
class tcp_server : public node
{
    struct _member;
    hardptr<_member> _m;
public:
    CREATE_H( tcp_server, int port, int num_of_client );
    ~tcp_server( );
    bool init( int port, int num_of_client );
    void update( float delta )override;
public:
    void write( network_handle const& handle, std::string const& message, std::function<void( )> on_send = nullptr );
    void write( network_handle const& handle, char const* message, size_t size, std::function<void( )> on_send = nullptr );
    void speech( std::string const& message, std::function<void( )> on_send = nullptr );
    void speech( char const* message, size_t size, std::function<void( )> on_send = nullptr );
    void close( network_handle const& handle );
    void on( std::string const& name, std::function<void( network_handle const&, Json::Value )> const& func );
public:
    std::vector<network_handle> get_clients( );
public:
    std::function<void( )> on_startup_failed;
    std::function<void( )> on_connections_overflow;
    std::function<void( network_handle const& )> on_send_failed;
    std::function<void( network_handle const&, char const*, size_t )> on_readed;
    std::function<void( network_handle const&, Json::Value )> on_received_json;
    std::map<std::string, std::function<void( network_handle const&, Json::Value )>> on_received_named_json;
    std::function<void( network_handle const& )> on_client_disconnected;
    std::function<void( network_handle const&, boost::system::error_code const& )> on_errored;
};
}
}