#pragma once
#include <string>
#include <treelike/forward.h>
namespace treelike
{
namespace network
{
class network_handle
{
public:
    network_handle( std::string const& ip_address, int const& port );
public:
    operator bool() const noexcept;
    bool operator==( network_handle const& other ) const noexcept;
    bool operator!=( network_handle const& other ) const noexcept;
    bool operator<( network_handle const& other ) const noexcept;
public:
    std::string const& ip_address;
    int const& port;
};
}
}