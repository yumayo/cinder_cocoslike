#include <treelike/action/rotate_by.h>
#include <treelike/node.h>
using namespace cinder;
namespace treelike
{
namespace action
{
CREATE_CPP( rotate_by, float duration, float radian, cinder::vec3 axis )
{
    CREATE( rotate_by, duration, radian, axis );
}
bool rotate_by::init( float duration, float radian, cinder::vec3 axis )
{
    _duration = duration;
    _init_rotation = radian;
    _init_axis = axis;
    return true;
}
void rotate_by::setup( )
{
    rotate_to::setup( );
    _rotation = _init_rotation + _start_rotation;
    _axis = _init_axis + _start_axis;
}
void rotate_by::restart( )
{
    rotate_to::restart( );
    setup( );
}
}
}