#include "tcp_client_member.h"
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "utility/assert_log.h"
#include "utility/base64.h"
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
void tcp_client::_member::write( char const* begin, size_t byte, std::function<void( )> on_send )
{
    auto writable_string = std::string( "#B#G#I#N#E#" ) + utility::base64_encode( begin, byte ) + std::string( "#E#N#D#" );
    asio::async_write(
        socket,
        asio::buffer( writable_string.data( ), writable_string.size( ) ),
        [ this, on_send ] ( const asio::error_code& e, size_t bytes_transferred )
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
            receive_buffer += std::string( buffer.data( ), bytes_transferred );
            std::fill_n( buffer.begin( ), bytes_transferred, 0 );
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
void tcp_client::_member::update( )
{
    while ( !receive_buffer.empty( ) )
    {
        auto find_position = receive_buffer.find( "#B#G#I#N#E#" );
        if ( find_position == std::string::npos ) // 自作プロトコルではないもの。
        {
            utility::log( "自作プロトコルではない情報を受信しました。" );
            receive_buffer.clear( );
        }
        else
        {
            receive_buffer = receive_buffer.substr( find_position + sizeof( "#B#G#I#N#E#" ) - 1 );
            find_position = receive_buffer.find( "#E#N#D#" );
            if ( find_position != std::string::npos ) // 見つかったら
            {
                auto str = receive_buffer.substr( 0, find_position ); // 見つかった場所までを切り取ります。
                receive_buffer = receive_buffer.substr( find_position + sizeof( "#E#N#D#" ) - 1 ); // 残りを詰め直す。この時点でendまでが切り取られ次のデータになる。
                try // base64でデコードします。
                {
                    auto receive_data = utility::base64_decode( str );
                    if ( parent.on_readed ) parent.on_readed( receive_data.first.get( ), receive_data.second );
                    Json::Value root;
                    if ( Json::Reader( ).parse( std::string( receive_data.first.get( ), receive_data.second ), root ) )
                    {
                        if ( parent.on_received_json ) parent.on_received_json( root );
                        auto itr = parent.on_received_named_json.find( root["name"].asString( ) );
                        if ( itr != std::end( parent.on_received_named_json ) )
                        {
                            if ( itr->second ) itr->second( root );
                        }
                    }
                }
                catch ( std::exception& e )
                {
                    utility::log( "base64でエンコードされていないデータを受信しました。" );
                }
            }
            else // 見つからない場合まだデータが途中ということになります。
            {
                return;
            }
        }
    }
}
}
