#include <treelike/network/tcp_server_member.h>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <treelike/utility/assert_log.h>
#include <treelike/utility/base64.h>
using namespace treelike::utility;
using tcp = boost::asio::ip::tcp;
namespace treelike
{
namespace network
{
tcp_server::_member::_member( tcp_server & parent, int port, int maximum_clients )
    : parent( parent )
    , io( )
    , acceptor( )
    , sockets( )
    , maximum_clients( std::max( 1, maximum_clients ) )
{
    acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>( io, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4( ), port ) );
    // 一つはクライアントの接続を切る役割を持たせています。
    for ( int i = 0; i < maximum_clients + 1; ++i )
    {
        sockets.emplace_back( std::make_unique<tcp_socket>( io ) );
    }
    int a = 0;
}
void tcp_server::_member::async_accept( tcp_socket& sock_obj )
{
    log( "【tcp_server】ソケットの準備" );
    acceptor->async_accept( sock_obj.socket, [ this, &sock_obj ] ( boost::system::error_code const& e )
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
                sock_obj.set_client_handle( sock_obj.socket.remote_endpoint( ).address( ).to_string( ), sock_obj.socket.remote_endpoint( ).port( ) );
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
void tcp_server::_member::write( tcp_socket & sock_obj, char const* begin, size_t byte, std::function<void( )> on_send )
{
    size_t num = byte / 12288;
    std::vector<int> sub;
    sub.emplace_back( 0 );
    for ( size_t i = 0; i < num; ++i )
    {
        sub.emplace_back( 12288 );
    }
    sub.emplace_back( byte - 12288 * num );

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
            sock_obj.socket,
            boost::asio::buffer( writable_string.data( ), writable_string.size( ) ),
            [ this, &sock_obj, on_send ] ( const boost::system::error_code& e, size_t bytes_transferred )
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
}
void tcp_server::_member::read( tcp_socket & sock_obj )
{
    boost::asio::async_read(
        sock_obj.socket,
        boost::asio::buffer( sock_obj.buffer ),
        boost::asio::transfer_at_least( 1 ), // １バイトでもデータが送られてきたら、読み込みを開始します。
        [ this, &sock_obj ] ( const boost::system::error_code& e, size_t bytes_transferred )
    {
        if ( e && bytes_transferred == 0 )
        {
            if ( e == boost::asio::error::eof )
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
            sock_obj.stack += std::string( sock_obj.buffer.data( ), bytes_transferred );
            std::fill_n( sock_obj.buffer.begin( ), bytes_transferred, 0 );
            read( sock_obj );
        }
    } );
}
void tcp_server::_member::on_errored( tcp_socket& sock_obj, boost::system::error_code const & e )
{
    if ( parent.on_errored ) parent.on_errored( sock_obj.handle, e );
}
void tcp_server::_member::close_with_async( tcp_socket & sock_obj )
{
    // クライアントがいなくなったソケットは、もう一度接続します。
    sock_obj.close( );
    async_accept( sock_obj );
}
void tcp_server::_member::find_run( network_handle const& handle, std::function<void( tcp_socket& )> call )
{
    auto itr = std::find_if( std::begin( sockets ), std::end( sockets ), [ handle ] ( std::unique_ptr<tcp_socket>& object )
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
void tcp_server::_member::update( )
{
    for ( auto& socket : sockets )
    {
        while ( !socket->stack.empty( ) )
        {
            auto begin_position = socket->stack.find( "#B#G#I#N#E#" );
            if ( begin_position != 0 ) // 自作プロトコルではないもの。
            {
                utility::log( "自作プロトコルではない情報を受信しました。" );
                socket->stack.clear( );
            }
            else
            {
                auto end_position = socket->stack.find( "#E#N#D#" );
                if ( end_position != std::string::npos )
                {
                    auto str = socket->stack.substr( begin_position + sizeof( "#B#G#I#N#E#" ) - 1, end_position - ( begin_position + sizeof( "#B#G#I#N#E#" ) - 1 ) );
                    socket->stack = socket->stack.substr( end_position + sizeof( "#E#N#D#" ) - 1 ); // 残りを詰め直す。
                    try
                    {
                        auto receive_data = utility::base64_decode( str );
                        if ( parent.on_readed ) parent.on_readed( socket->handle, receive_data.first.get( ), receive_data.second );
                        Json::Value root;
                        if ( Json::Reader( ).parse( std::string( receive_data.first.get( ), receive_data.second ), root ) )
                        {
                            if ( parent.on_received_json ) parent.on_received_json( socket->handle, root );
                            auto itr = parent.on_received_named_json.find( root["name"].asString( ) );
                            if ( itr != std::end( parent.on_received_named_json ) )
                            {
                                if ( itr->second ) itr->second( socket->handle, root );
                            }
                        }
                    }
                    catch ( std::exception& )
                    {
                        utility::log( "base64でエンコードされていないデータを受信しました。" );
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
}
}
}