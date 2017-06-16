#include "udp_connection_member.h"
#include "jsoncpp/json.h"
#include "utility/string_utility.h"
namespace network
{
udp_connection::member::member( udp_connection & server, udp::endpoint const & end_point )
    : _connection( server )
    , _udp_socket( _io_service, end_point )
    , _network_factory( server )
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
udp_connection::member::member( udp_connection& server, int const & port_num )
    : udp_connection::member( server, udp::endpoint( udp::v4( ), port_num ) )
{
}
udp_connection::member::~member( )
{
    close( );
}
void udp_connection::member::write( network_handle const & handle, Json::Value const & send_data )
{
    write( handle, Json::FastWriter( ).write( send_data ) );
}
void udp_connection::member::write( network_handle const & handle, std::string const & send_data )
{
    write( handle, send_data.c_str( ), send_data.size( ) );
}
void udp_connection::member::write( network_handle const & handle, char const * send_data )
{
    write( handle, std::string( send_data ) );
}
void udp_connection::member::write( network_handle const & handle, char const * send_data, size_t const & send_data_byte )
{
    try
    {
        udp::resolver resolver( _io_service );
        udp::resolver::query query( udp::v4( ),
                                    handle->ip_address,
                                    boost::lexical_cast<std::string>( handle->port ) );
        _udp_socket.send_to( asio::buffer( send_data, send_data_byte ),
                             resolver.resolve( query )->endpoint( ) );
        if ( _connection.on_sended )_connection.on_sended( );
    }
    catch ( asio::error_code& error )
    {
        utility::log_network( handle->ip_address, handle->port,
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
    _io_service.restart( );
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

    _network_factory.update( delta_second );

    for ( auto& data : _receive_deque )
    {
        auto handle = _network_factory.regist( data.first.address( ).to_string( ),
                                               data.first.port( ) );
        handle->timeout_restart( );

        // データを受け取ったら呼び出されます。
        if ( _connection.on_received ) _connection.on_received( handle,
                                                                data.second.data( ), data.second.size( ) );

        Json::Value root;
        if ( Json::Reader( ).parse( std::string( data.second.data( ), data.second.size( ) ), root ) )
        {
            // 通常用のjson関数を呼び出します。
            if ( _connection.on_received_json )_connection.on_received_json( handle, root );

            // map式のjson関数を呼び出します。
            auto itr = _connection.on_received_named_json.find( root["name"].asString( ) );
            if ( itr != std::end( _connection.on_received_named_json ) )
            {
                if ( itr->second ) itr->second( handle, root );
            }
        }
    }
    _receive_deque.clear( );
}
void udp_connection::member::_receive( )
{
    _udp_socket.async_receive_from( asio::buffer( _remote_buffer ),
                                    _remote_endpoint,
                                    [ this ] ( const asio::error_code& e, size_t bytes_transferred )
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

            _receive_deque.emplace_back( std::make_pair( _remote_endpoint, std::vector<char>( _remote_buffer.begin( ), _remote_buffer.begin( ) + bytes_transferred ) ) );

            std::fill_n( _remote_buffer.begin( ), bytes_transferred, 0 );
        }
    } );
}
bool udp_connection::member::destroy_client( network_handle const & handle )
{
    return _network_factory.destroy_client( handle );
}
network_handle udp_connection::member::regist_client( std::string const& ip_address, int const& port )
{
    return _network_factory.regist( ip_address, port );
}
std::list<std::shared_ptr<network_object>>& udp_connection::member::get_clients( )
{
    return _network_factory.get_clients( );
}
int udp_connection::member::get_port( )
{
    return _udp_socket.local_endpoint( ).port( );
}
}
