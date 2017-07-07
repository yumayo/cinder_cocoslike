#pragma once
#include <treelike/action/finite_time_action.h>
namespace treelike
{
namespace action
{
class call_func : public finite_time_action
{
public:
    CREATE_H( call_func, std::function<void( )> call );
    bool init( std::function<void( )> call );
public:
    virtual float update( float delta ) override;
private:
    std::function<void( )> _call;
};
}
}