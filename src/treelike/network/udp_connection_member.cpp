#include <treelike/network/udp_connection_member.h>
#include <treelike/utility/string.h>
#include <treelike/utility/base64.h>
namespace treelike
{
namespace network
{
udp_connection::member::member( udp_connection & server, udp::endpoint const & end_point )
    : _connection( server )
    , _udp_socket( _io_service, end_point )
{
    _remote_buffer.fill( 0 );

    _update_io_service = std::thread( [ this ]
    {
        while ( !_is_pause )
        {
            _receive( );
            _io_service.run( );
            _io_service.reset( );
        }
    } );
}
udp_connection::member::member( udp_connection & server )
    : udp_connection::member( server, udp::endpoint( udp::v4( ), 0 ) )
{
}
udp_connection::member::member( udp_connection& server, int port_num )
    : udp_connection::member( server, udp::endpoint( udp::v4( ), port_num ) )
{
}
udp_connection::member::~member( )
{
    close( );
}
void udp_connection::member::write( network_handle handle, Json::Value const & send_data )
{
    write( handle, Json::FastWriter( ).write( send_data ) );
}
void udp_connection::member::write( network_handle handle, std::string const & send_data )
{
    write( handle, send_data.c_str( ), send_data.size( ) );
}
void udp_connection::member::write( network_handle handle, char const * send_data )
{
    write( handle, std::string( send_data ) );
}
void udp_connection::member::write( network_handle handle, char const * send_data, size_t const & send_data_byte )
{
    try
    {
        udp::resolver resolver( _io_service );
        udp::resolver::query query( udp::v4( ),
                                    handle.ip_address,
                                    boost::lexical_cast<std::string>( handle.port ) );
        auto writable_string = std::string( "#B#G#I#N#E#" ) + utility::base64_encode( send_data, send_data_byte ) + std::string( "#E#N#D#" );
        _udp_socket.send_to( boost::asio::buffer( writable_string.data( ), writable_string.size( ) ),
                             resolver.resolve( query )->endpoint( ) );
        if ( _connection.on_sended )_connection.on_sended( );
    }
    catch ( boost::system::error_code& error )
    {
        utility::log_network( handle.ip_address, handle.port,
                              "データを送れませんでした。: %s", error.message( ).c_str( ) );
        if ( _connection.on_send_failed )_connection.on_send_failed( );
    }
}
void udp_connection::member::close( )
{
    _is_pause = true;
    _io_service.stop( );
    _update_io_service.join( );
    _udp_socket.close( );
}
void udp_connection::member::open( )
{
    _is_pause = false;
    _udp_socket.open( udp::v4( ) );
    _update_io_service = std::thread( [ this ]
    {
        while ( !_is_pause )
        {
            _receive( );
            _io_service.run( );
            _io_service.reset( );
        }
    } );
}
void udp_connection::member::update( float delta_second )
{
    utility::scoped_mutex m( _mutex );

    for ( auto& receive_buffer : _receive_buffers )
    {
        auto handle = network_handle( receive_buffer.first.address( ).to_string( ), receive_buffer.first.port( ) );

        while ( !receive_buffer.second.empty( ) )
        {
            auto begin_position = receive_buffer.second.find( "#B#G#I#N#E#" );
            if ( begin_position != 0 ) // 自作プロトコルではないもの。
            {
                utility::log( "自作プロトコルではない情報を受信しました。" );
                receive_buffer.second.clear( );
            }
            else
            {
                auto end_position = receive_buffer.second.find( "#E#N#D#" );
                if ( end_position != std::string::npos ) // 見つかったら
                {
                    auto str = receive_buffer.second.substr( begin_position + sizeof( "#B#G#I#N#E#" ) - 1, end_position - ( begin_position + sizeof( "#B#G#I#N#E#" ) - 1 ) ); // 見つかった場所までを切り取ります。
                    receive_buffer.second = receive_buffer.second.substr( end_position + sizeof( "#E#N#D#" ) - 1 ); // 残りを詰め直す。この時点でendまでが切り取られ次のデータになる。
                    try // base64でデコードします。
                    {
                        auto receive_data = utility::base64_decode( str );
                        if ( _connection.on_received ) _connection.on_received( handle,
                                                                                receive_data.first.get( ), receive_data.second );
                        Json::Value root;
                        if ( Json::Reader( ).parse( std::string( receive_data.first.get( ), receive_data.second ), root ) )
                        {
                            if ( _connection.on_received_json )_connection.on_received_json( handle, root );
                            auto itr = _connection.on_received_named_json.find( root["name"].asString( ) );
                            if ( itr != std::end( _connection.on_received_named_json ) )
                            {
                                if ( itr->second ) itr->second( handle, root );
                            }
                        }
                    }
                    catch ( std::exception& )
                    {
                        utility::log( "base64でエンコードされていないデータを受信しました。" );
                    }
                }
            }
        }
    }
}
void udp_connection::member::_receive( )
{
    _udp_socket.async_receive_from( boost::asio::buffer( _remote_buffer ),
                                    _remote_endpoint,
                                    [ this ] ( const boost::system::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            utility::log_network( _remote_endpoint.address( ).to_string( ), _remote_endpoint.port( ),
                                  "データを受け取れませんでした。: %s", e.message( ).c_str( ) );
            if ( _connection.on_receive_failed ) _connection.on_receive_failed( );
        }
        else
        {
            utility::scoped_mutex m( _mutex );

            _receive_buffers[_remote_endpoint] += std::string( _remote_buffer.data( ), bytes_transferred );

            std::fill_n( _remote_buffer.begin( ), bytes_transferred, 0 );
        }
    } );
}
int udp_connection::member::get_port( )
{
    return _udp_socket.local_endpoint( ).port( );
}
}

}