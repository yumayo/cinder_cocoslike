#pragma once
#include <treelike/node.h>
#include <boost/system/error_code.hpp>
#include <jsoncpp/json.h>
namespace treelike
{
namespace network
{
class tcp_client : public node
{
    struct _member;
    hardptr<_member> _m;
public:
    CREATE_H( tcp_client, std::string const& ip_address, int port );
    bool init( std::string const& ip_address, int port );
    ~tcp_client( );
    void update( float delta ) override;
public:
    void write( std::string const& message, std::function<void( )> on_send = nullptr );
    void write( char const* message, size_t size, std::function<void( )> on_send = nullptr );
    int get_port( );
    void on( std::string const& name, std::function<void( Json::Value root )> const& func );
public:
    // 接続が成功したら呼ばれます。
    std::function<void( )> on_connection;
    // データを送れなかったときに呼ばれます。
    std::function<void( )> on_send_failed;
    // データが送られてきたときに呼ばれます。
    std::function<void( char const*, size_t )> on_readed;
    // 
    std::function<void( Json::Value root )> on_received_json;
    // 
    std::map<std::string, std::function<void( Json::Value root )>> on_received_named_json;
    // 接続できなかったときに呼ばれます。
    std::function<void( )> on_connect_failed;
    // 接続が切れたときに呼ばれます。
    std::function<void( )> on_disconnected;
    // ソケットが閉じたときに呼ばれます。
    std::function<void( )> on_closed;
    // その他のエラーが出たときに呼ばれます。
    std::function<void( boost::system::error_code const& )> on_errored;
};
}
}