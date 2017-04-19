#pragma once
#include <string>
#include "cinder/app/App.h"
namespace utility
{
std::string format( char const* format, ... );
void log( char const* str, ... );
void log_with_time_stamp( char const* str, ... );
void log_network( std::string const& ip_address, int const& port, char const* str, ... );
void log_data( char const * data, size_t size );
}
namespace cinder
{
namespace app
{
// データ書き出しのディレクトリを取得
// ディレクトリ構造には対応していません。
std::string getWritableDirectry( );
std::string getWritablePath( std::string const& filename );
std::string loadString( std::string const& relative_path );
std::string getSystemTimeNamed( );
void writeFile( std::string const& filename, std::string const& document );
}
}