#pragma once
#include <treelike/node.h>
namespace treelike
{
namespace renderer
{
class camera_perspective : public node
{
public:
    CREATE_H( camera_perspective, cinder::vec3 const& size );
    bool init( cinder::vec3 const& size );
    void render( ) override;
};
}
}