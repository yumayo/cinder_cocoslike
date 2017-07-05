#include <treelike/network/network_handle.h>
namespace treelike
{
namespace network
{
network_handle::network_handle( std::string const & ip_address, int const & port )
    : ip_address( ip_address )
    , port( port )
{

}
network_handle::operator bool( ) const noexcept
{
    return ( !this->ip_address.empty( ) ) && ( this->port != 0 );
}
bool network_handle::operator==( network_handle const & other ) const noexcept
{
    return this->ip_address == other.ip_address && this->port == other.port;
}
bool network_handle::operator!=( network_handle const & other ) const noexcept
{
    return !( this->ip_address == other.ip_address && this->port == other.port );
}
bool network_handle::operator<( network_handle const & other ) const noexcept
{
    return this->ip_address < other.ip_address && this->port < other.port;
}
}
}