#include <treelike/action/fade_out.h>
#include <treelike/node.h>
using namespace cinder;
namespace treelike
{
namespace action
{
CREATE_CPP( fade_out, float duration )
{
    CREATE( fade_out, duration );
}
bool fade_out::init( float duration )
{
    finite_time_action::init( duration );
    return true;
}
void fade_out::setup( )
{
    _opacity = _target->get_opacity( );
}
void fade_out::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    auto const to = 0.0F;
    auto const from = _opacity;
    auto const temp = ease_liner( t, from, to );
    _target->set_opacity( temp );
}
}
}