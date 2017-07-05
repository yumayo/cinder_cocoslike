#pragma once
#include <treelike/action/finite_time_action.h>
#include <cinder/Vector.h>
namespace treelike
{
namespace action
{
class rotate_to : public finite_time_action
{
public:
    CREATE_H( rotate_to, float duration, float radian );
    bool init( float duration, float radian );
private:
    void setup( ) override;
    void step( float t ) override;
protected:
    float _start_rotation = 0.0F;
    float _rotation = 0.0F;
};
}
}