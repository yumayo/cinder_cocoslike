#include <treelike/action/move_by.h>
#include <treelike/node.h>
using namespace cinder;
namespace treelike
{
namespace action
{
CREATE_CPP( move_by, float duration, cinder::vec2 position )
{
    CREATE( move_by, duration, position );
}
bool move_by::init( float duration, cinder::vec2 position )
{
    _duration = duration;
    _init_position = vec3( position, 0.0F );
    return true;
}
CREATE_CPP( move_by, float duration, cinder::vec3 position_3d )
{
    CREATE( move_by, duration, position_3d );
}
bool move_by::init( float duration, cinder::vec3 position_3d )
{
    _duration = duration;
    _init_position = position_3d;
    return true;
}
void move_by::setup( )
{
    move_to::setup( );
    _position = _init_position + _start_position;
}
void move_by::restart( )
{
    move_to::restart( );
    setup( );
}
}
}