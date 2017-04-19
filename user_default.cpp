#include "user_default.h"
#include "utility/assert_log.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
#include "utility/string_utility.h"
#include <fstream>
#include <boost/filesystem.hpp>
using namespace cinder;
user_default* user_default::_instans = nullptr;
Json::Value & user_default::get_root( )
{
    return _root;
}
void user_default::save( )
{
    Json::StyledWriter writer;
    auto data = writer.write( _root );
    std::ofstream output( cinder::app::getWritablePath( _target_file_name ) );
    output << data;
}
user_default* user_default::get_instans( )
{
    if ( !_instans ) _instans = new user_default;
    return _instans;
}
void user_default::remove_instans( )
{
    get_instans( )->save( );
    delete _instans;
    _instans = nullptr;
}
user_default::user_default( )
{
    std::string document = boost::filesystem::exists( app::getWritablePath( _target_file_name ) )
        ? app::loadString( _target_file_name )
        : "{}";
    Json::Reader reader;
    assert_log( reader.parse( document, _root ),
                "無効なJsonファイルです。",
                return );

}
