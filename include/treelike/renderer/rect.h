#pragma once
#include <treelike/node.h>
namespace treelike
{
namespace renderer
{
    class rect : public node
    {
    public:
        CREATE_H( rect, cinder::vec2 const& size );
        bool init( cinder::vec2 const& size );
        void render( ) override;
    };
}
}