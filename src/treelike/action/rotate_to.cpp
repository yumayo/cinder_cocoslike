#include <treelike/action/rotate_to.h>
#include <treelike/node.h>
using namespace cinder;
namespace treelike
{
namespace action
{
CREATE_CPP( rotate_to, float duration, float radian, cinder::vec3 axis )
{
    CREATE( rotate_to, duration, radian, axis );
}
bool rotate_to::init( float duration, float radian, cinder::vec3 axis )
{
    _duration = duration;
    _rotation = radian;
    _axis = axis;
    return true;
}
void rotate_to::setup( )
{
    _start_rotation = _target->get_rotation( );
    _start_axis = _target->get_axis( );
}
void rotate_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    {
        auto const to = _rotation;
        auto const from = _start_rotation;
        auto const temp = ease_liner( t, from, to );
        _target->set_rotation( temp );
    }
    {
        auto const to = _axis;
        auto const from = _start_axis;
        auto const temp = vec3( ease_liner( t, from.x, to.x ), ease_liner( t, from.y, to.y ), ease_liner( t, from.z, to.z ) );
        _target->set_axis( temp );
    }
}
void rotate_to::restart( )
{
    finite_time_action::restart( );
    setup( );
}
}
}