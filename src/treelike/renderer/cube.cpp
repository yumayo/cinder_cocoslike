#include <treelike/renderer/cube.h>
#include <cinder/gl/gl.h>
#include <treelike/utility/assert_log.h>
using namespace cinder;
namespace treelike
{
namespace renderer
{
CREATE_CPP( cube, cinder::vec3 const & size )
{
    CREATE( cube, size );
}
bool cube::init( cinder::vec3 const& size )
{
    set_anchor_point_3d( vec3( 0.5F ) );
    set_pivot_3d( vec3( 0.5F ) );
    set_content_size_3d( size );
    return true;
}
void cube::render( )
{
    gl::drawCube( get_content_size_3d( ) * get_anchor_point_3d( ), get_content_size_3d( ) );
}
}
}