#pragma once
#include <list>
#include "network_object.h"
#include "udp_connection.h"

namespace network
{
class network_factory
{
protected:
    udp_connection& _connection;
    std::list<std::shared_ptr<network_object>> _network_objects;
public:
    network_factory( ) = delete;
    network_factory( udp_connection& connection );
    virtual ~network_factory( ) { }
    network_handle regist( std::string const& ip_address, int const& port );
    bool destroy_client( network_handle handle );
    std::list<std::shared_ptr<network_object>>::iterator find_network_object( network_handle handle );
    std::list<std::shared_ptr<network_object>>& get_clients( );
    void update( float delta_second );
};
}