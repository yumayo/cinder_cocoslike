#include <treelike/utility/file_system.h>
namespace treelike
{
namespace utility
{
void file_system::search( std::string const & path )
{
    _root_relative_path = path;
    search_path( path );
}
std::vector<std::string>& file_system::get_names( )
{
    return _names;
}
void file_system::search_directory( std::tr2::sys::path const & path )
{
    search_path( path );
}
void file_system::search_file( std::tr2::sys::path const & path )
{
    _names.emplace_back( path.string( ).substr( _root_relative_path.size( ), std::string::npos ) );
}
void file_system::search_path( std::tr2::sys::path const & path )
{
    std::tr2::sys::path p( path ); // 列挙の起点
    std::for_each( std::tr2::sys::directory_iterator( p ), std::tr2::sys::directory_iterator( ), [ this ] ( std::tr2::sys::path const& p )
    {
        // ファイルなら...
        if ( std::tr2::sys::is_regular_file( p ) )
        {
            search_file( p );
        }
    } );

    p = std::tr2::sys::path( path ); // 列挙の起点
    std::for_each( std::tr2::sys::directory_iterator( p ), std::tr2::sys::directory_iterator( ), [ this ] ( std::tr2::sys::path const& p )
    {
        // ディレクトリなら...
        if ( std::tr2::sys::is_directory( p ) )
        {
            search_directory( p );
        }
    } );
}
}

}