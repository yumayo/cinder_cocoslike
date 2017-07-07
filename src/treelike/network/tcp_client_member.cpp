#include <treelike/network/tcp_client_member.h>
#include <boost/lexical_cast.hpp>
#include <treelike/utility/assert_log.h>
#include <treelike/utility/base64.h>
using namespace treelike::utility;
using tcp = boost::asio::ip::tcp;
namespace treelike
{
namespace network
{
void tcp_client::_member::connect( )
{
    socket.async_connect(
        tcp::endpoint( boost::asio::ip::address::from_string( ip_address ), boost::lexical_cast<int>( port ) ),
        [ this ] ( const boost::system::error_code& e )
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
    size_t num = byte / ( 256 * 48 );
    std::vector<int> sub;
    sub.emplace_back( 0 );
    for ( size_t i = 0; i < num; ++i )
    {
        sub.emplace_back( 256 * 48 );
    }
    sub.emplace_back( byte - ( 256 * 48 ) * num );

    for ( size_t i = 0; i < sub.size( ) - 1; ++i )
    {
        std::string writable_string;
        if ( i == 0 )
        {
            writable_string += std::string( "#B#G#I#N#E#" );
        }
        int offset = 0;
        for ( size_t j = 0; j <= i; ++j ) offset += sub[j];
        writable_string += utility::base64_encode( begin + offset, sub[i + 1] );
        if ( i == sub.size( ) - 1 - 1 )
        {
            writable_string += std::string( "#E#N#D#" );
        }
        boost::asio::async_write(
            socket,
            boost::asio::buffer( writable_string.data( ), writable_string.size( ) ),
            [ this, on_send ] ( const boost::system::error_code& e, size_t bytes_transferred )
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
}
void tcp_client::_member::read( )
{
    boost::asio::async_read(
        socket,
        boost::asio::buffer( buffer ),
        boost::asio::transfer_at_least( 1 ),
        [ this ] ( const boost::system::error_code& e, size_t bytes_transferred )
    {
        if ( e && bytes_transferred == 0 )
        {
            if ( e == boost::asio::error::eof )
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
void tcp_client::_member::error( boost::system::error_code const& e )
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
        auto begin_position = receive_buffer.find( "#B#G#I#N#E#" );
        if ( begin_position != 0 ) // 自作プロトコルではないもの。
        {
            utility::log( "自作プロトコルではない情報を受信しました。" );
            receive_buffer.clear( );
        }
        else
        {
            auto end_position = receive_buffer.find( "#E#N#D#" );
            if ( end_position != std::string::npos ) // 見つかったら
            {
                auto str = receive_buffer.substr( begin_position + sizeof( "#B#G#I#N#E#" ) - 1, end_position - ( begin_position + sizeof( "#B#G#I#N#E#" ) - 1 ) ); // 見つかった場所までを切り取ります。
                receive_buffer = receive_buffer.substr( end_position + sizeof( "#E#N#D#" ) - 1 ); // 残りを詰め直す。この時点でendまでが切り取られ次のデータになる。
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
                catch ( std::exception& )
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

}