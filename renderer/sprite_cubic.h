#pragma once
#include "sprite.h"
namespace renderer
{
    class sprite_cubic : public sprite
    {
    public:
        CREATE_H( sprite_cubic, std::string const& relative_path );
        bool init( std::string const& relative_path );
    };
}