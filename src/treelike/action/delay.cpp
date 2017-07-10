#include <treelike/action/finite_time_action.h>
#include <treelike/node.h>
#include <treelike/action/delay.h>
namespace treelike
{
namespace action
{
CREATE_CPP( delay, float duration )
{
    CREATE( delay, duration );
}
bool delay::init( float duration )
{
    finite_time_action::init( duration );
    return true;
}
}
}