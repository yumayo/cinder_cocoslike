#include "tcp_server_member.h"
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "utility/assert_log.h"
using namespace utility;
using tcp = asio::ip::tcp;
namespace network
{
tcp_server::_member::_member( tcp_server & parent, std::string const & port, int num_of_client )
    : parent( parent )
    , io( )
    , acceptor( )
    , sockets( )
    , port( port )
{
    assert_log( 1 <= num_of_client, "クライアントの数は一つ以上でないといけません。", return );
    acceptor = std::make_unique<asio::ip::tcp::acceptor>( io, asio::ip::tcp::endpoint( asio::ip::tcp::v4( ), boost::lexical_cast<int>( port ) ) );
    // 一つはクライアントの接続を切る役割を持たせています。
    for ( int i = 0; i < num_of_client + 1; ++i )
    {
        sockets.emplace_back( std::make_unique<socket_object>( io ) );
    }
    int a = 0;
}
void tcp_server::_member::async_accept( socket_object& sock_obj )
{
    log( "【tcp_server】ソケットの準備" );
    acceptor->async_accept( sock_obj.socket, [ this, &sock_obj ] ( asio::error_code const& e )
    {
        if ( e )
        {
            log( "【tcp_server】接続は無効になりました。: %s", e.message( ).c_str( ) );
            if ( parent.on_startup_failed ) parent.on_startup_failed( );
        }
        else
        {
            log( "【tcp_server】接続を受け付けました。" );
            if ( is_max( ) )
            {
                if ( parent.on_connections_overflow ) parent.on_connections_overflow( );
                close_with_async( sock_obj );
            }
            else
            {
                log( "【tcp_server】接続成功！: %s, %d", sock_obj.socket.remote_endpoint( ).address( ).to_string( ).c_str( ), sock_obj.socket.remote_endpoint( ).port( ) );
                sock_obj.connect( sock_obj.socket.remote_endpoint( ).address( ).to_string( ), boost::lexical_cast<std::string>( sock_obj.socket.remote_endpoint( ).port( ) ) );
                read( sock_obj );
            }
        }
    } );
}
bool tcp_server::_member::is_max( )
{
    int num = 0;
    for ( auto& obj : sockets )
    {
        if ( obj->socket.is_open( ) ) num++;
    }
    return sockets.size( ) == num;
}
void tcp_server::_member::read( socket_object & sock_obj )
{
    asio::async_read(
        sock_obj.socket,
        asio::buffer( sock_obj.buffer ),
        asio::transfer_at_least( 1 ), // １バイトでもデータが送られてきたら、読み込みを開始します。
        [ this, &sock_obj ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e && bytes_transferred == 0 )
        {
            if ( e == asio::error::eof )
            {
                log( "【tcp_server】クライアントが接続を切りました。: %s", e.message( ).c_str( ) );
                if ( parent.on_client_disconnected ) parent.on_client_disconnected( sock_obj.handle );
            }
            else
            {
                log( "【tcp_server】無効なアクセスです。: %s", e.message( ).c_str( ) );
                on_errored( sock_obj, e );
            }
            close_with_async( sock_obj );
        }
        else
        {
            if ( parent.on_readed ) parent.on_readed( sock_obj.handle, sock_obj.buffer.data( ), bytes_transferred );

            Json::Value root;
            if ( Json::Reader( ).parse( std::string( sock_obj.buffer.data( ), bytes_transferred ), root ) )
            {
                // 通常用のjson関数を呼び出します。
                if ( parent.on_received_json ) parent.on_received_json( sock_obj.handle, root );

                // map式のjson関数を呼び出します。
                auto itr = parent.on_received_named_json.find( root["name"].asString( ) );
                if ( itr != std::end( parent.on_received_named_json ) )
                {
                    if ( itr->second ) itr->second( sock_obj.handle, root );
                }
                else
                {
                    log( "【tcp_server】名前に一致するjsonデータが見つかりませんでした。" );
                }
            }
            std::fill_n( sock_obj.buffer.begin( ), bytes_transferred, 0 );

            // クライアントからの接続が切れるまで無限に受け取り続けます。
            read( sock_obj );
        }
    } );
}
void tcp_server::_member::write( socket_object& sock_obj, asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    asio::async_write(
        sock_obj.socket,
        buffer,
        [ this, &sock_obj, on_send, buffer ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            log( "【tcp_server】送信できませんでした。: %s", e.message( ).c_str( ) );
            if ( parent.on_send_failed ) parent.on_send_failed( sock_obj.handle );
        }
        else
        {
            if ( on_send ) on_send( );
        }
    } );
}
void tcp_server::_member::on_errored( socket_object& sock_obj, asio::error_code const & e )
{
    if ( parent.on_errored ) parent.on_errored( sock_obj.handle, e );
}
void tcp_server::_member::close_with_async( socket_object & sock_obj )
{
    // クライアントがいなくなったソケットは、もう一度接続します。
    sock_obj.close( );
    async_accept( sock_obj );
}
void tcp_server::_member::find_run( client_handle const& handle, std::function<void( socket_object& )> call )
{
    auto itr = std::find_if( std::begin( sockets ), std::end( sockets ), [ handle ] ( std::unique_ptr<socket_object>& object )
    {
        return object->handle == handle;
    } );
    if ( itr != sockets.end( ) )
    {
        call( **itr );
    }
    else
    {
        log( "【tcp_server】名前と一致するクライアントが見つかりませんでした。" );
    }
}
}