#pragma once
#include <string>
#include <memory>
#include <utility>
namespace treelike
{
namespace utility
{
std::string base64_encode( char const* begin, size_t byte );
std::pair<std::unique_ptr<char [ ]>, size_t> base64_decode( std::string const& base64 );
}
}