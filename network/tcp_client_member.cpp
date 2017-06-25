#include "tcp_client_member.h"
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "utility/assert_log.h"
using namespace utility;
using tcp = asio::ip::tcp;
namespace network
{
void tcp_client::_member::connect( )
{
    socket.async_connect(
        tcp::endpoint( asio::ip::address::from_string( ip_address ), boost::lexical_cast<int>( port ) ),
        [ this ] ( const asio::error_code& e )
    {
        if ( e )
        {
            log( "【tcp_client】接続できませんでした。: %s", e.message( ).c_str( ) );
            if ( parent.on_connect_failed ) parent.on_connect_failed( );
            close( );
        }
        else
        {
            log( "【tcp_client】接続成功！" );
            if ( parent.on_connection ) parent.on_connection( );
            read( );
        }
    } );
}
void tcp_client::_member::write( asio::const_buffers_1 buffer, std::function<void( )> on_send )
{
    asio::async_write(
        socket,
        buffer,
        [ this, on_send, buffer ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            log( "【tcp_client】送信できませんでした。: %s", e.message( ).c_str( ) );
            if ( parent.on_send_failed ) parent.on_send_failed( );
            close( );
        }
        else
        {
            if ( on_send ) on_send( );
        }
    } );
}
void tcp_client::_member::read( )
{
    asio::async_read(
        socket,
        asio::buffer( buffer ),
        asio::transfer_at_least( 1 ),
        [ this ] ( const asio::error_code& e, size_t bytes_transferred )
    {
        if ( e && bytes_transferred == 0 )
        {
            if ( e == asio::error::eof )
            {
                log( "【tcp_client】サーバーが接続を切りました。: %s", e.message( ).c_str( ) );
                if ( parent.on_disconnected ) parent.on_disconnected( );
            }
            else
            {
                log( "【tcp_client】無効なアクセスです。: %s", e.message( ).c_str( ) );
                error( e );
            }
            close( );
        }
        else
        {
            if ( parent.on_readed ) parent.on_readed( buffer.data( ), bytes_transferred );

            Json::Value root;
            if ( Json::Reader( ).parse( std::string( buffer.data( ), bytes_transferred ), root ) )
            {
                // 通常用のjson関数を呼び出します。
                if ( parent.on_received_json ) parent.on_received_json( root );

                // map式のjson関数を呼び出します。
                auto itr = parent.on_received_named_json.find( root["name"].asString( ) );
                if ( itr != std::end( parent.on_received_named_json ) )
                {
                    if ( itr->second ) itr->second( root );
                }
            }

            std::fill_n( buffer.begin( ), bytes_transferred, 0 );

            // エラーじゃない限り無限に受け取りを続けます。
            read( );
        }
    } );
}
void tcp_client::_member::close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
void tcp_client::_member::error( asio::error_code const& e )
{
    if ( parent.on_errored ) parent.on_errored( e );
}
int tcp_client::_member::get_port( )
{
    return socket.local_endpoint( ).port( );
}
}
