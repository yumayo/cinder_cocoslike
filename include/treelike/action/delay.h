#pragma once
#include <treelike/action/finite_time_action.h>
namespace treelike
{
namespace action
{
class delay : public finite_time_action
{
public:
    CREATE_H( delay, float duration );

public:
    bool init( float duration );
};
}
}