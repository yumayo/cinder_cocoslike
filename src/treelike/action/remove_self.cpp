#include <treelike/action/remove_self.h>
#include <treelike/node.h>
namespace treelike
{
namespace action
{
CREATE_CPP( remove_self )
{
    CREATE( remove_self );
}
bool remove_self::init( )
{
    return true;
}
float remove_self::update( float delta )
{
    _target->remove_from_parent( );
    return delta;
}
}
}