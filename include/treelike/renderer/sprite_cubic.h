#pragma once
#include <treelike/renderer/sprite.h>
namespace treelike
{
namespace renderer
{
    class sprite_cubic : public sprite
    {
    public:
        CREATE_H( sprite_cubic, std::string const& relative_path );
        bool init( std::string const& relative_path );
    };
}
}