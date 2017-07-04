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
    _init_position = position;
    return true;
}
void move_by::setup( )
{
    _start_position = _target->get_position( );
    _position = _init_position + _start_position;
}
}
}