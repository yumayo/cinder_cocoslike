#pragma once
#include <treelike/action/finite_time_action.h>
namespace treelike
{
namespace action
{
class rotate_to : public finite_time_action
{
public:
    CREATE_H( rotate_to, float duration, float radian );
    bool init( float duration, float radian );
protected:
    void setup( ) override;
    void step( float t ) override;
    void restart( ) override;
protected:
    float _start_rotation = 0.0F;
    float _rotation = 0.0F;
};
}
}