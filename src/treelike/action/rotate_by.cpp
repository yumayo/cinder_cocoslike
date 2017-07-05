#include <treelike/action/rotate_by.h>
#include <treelike/node.h>
using namespace cinder;
namespace treelike
{
namespace action
{
CREATE_CPP( rotate_by, float duration, float radian )
{
    CREATE( rotate_by, duration, radian );
}
bool rotate_by::init( float duration, float radian )
{
    _duration = duration;
    _init_rotation = radian;
    return true;
}
void rotate_by::setup( )
{
    rotate_to::setup( );
    _rotation = _init_rotation + _start_rotation;
}
void rotate_by::restart( )
{
    rotate_to::restart( );
    setup( );
}
}
}