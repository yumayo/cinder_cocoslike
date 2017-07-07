#pragma once
#include <treelike/action/finite_time_action.h>
namespace treelike
{
namespace action
{
class timeline : public finite_time_action
{
public:
    CREATE_H( timeline );
    virtual ~timeline( ) { }
public:
    bool init( );

protected:
    std::vector<hardptr<finite_time_action>> _actions;
};
}
}