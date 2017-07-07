#pragma once
#include <treelike/node.h>
namespace treelike
{
namespace renderer
{
class cube : public node
{
public:
    CREATE_H( cube, cinder::vec3 const& size );
    bool init( cinder::vec3 const& size );
    void render( ) override;
};
}
}