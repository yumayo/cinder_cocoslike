#pragma once

#include <string>
using string = std::string;
#include <treelike/utility/user_pointer.hpp>
template<class type>
using hardptr = treelike::utility::hardptr<type>;
template<class type, class... args>
inline hardptr<type> make_hard( args const& ...argv )
{
    return std::make_shared<type>( argv... );
}
template<class type>
using softptr = treelike::utility::softptr<type>;

#define CREATE_H(__TYPE__, ...) static hardptr<__TYPE__> create(__VA_ARGS__)
#define CREATE_CPP(__TYPE__, ...) hardptr<__TYPE__> __TYPE__::create(__VA_ARGS__)
#define CREATE(__TYPE__, ...) \
auto ret = make_hard<__TYPE__>( ); \
if ( ret && ret->init( __VA_ARGS__ ) ); \
else ret.reset( ); \
return std::move( ret )
