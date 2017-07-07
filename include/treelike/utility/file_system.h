#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
namespace treelike
{
namespace utility
{
class file_system
{
    std::string _root_relative_path;
    std::vector<std::string> _names;
public:
    void search( std::string const& relative_path );
    std::vector<std::string>& get_names( );
private:
    void search_directory( std::tr2::sys::path const& path );
    void search_file( std::tr2::sys::path const& path );
    void search_path( std::tr2::sys::path const& path );
};
}


}