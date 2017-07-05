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
    CREATE_H( rotate_to, float duration, float radian, cinder::vec3 axis = cinder::vec3( 0, 0, 1 ) );
    bool init( float duration, float radian, cinder::vec3 axis = cinder::vec3( 0, 0, 1 ) );
protected:
    void setup( ) override;
    void step( float t ) override;
    void restart( ) override;
protected:
    float _start_rotation = 0.0F;
    float _rotation = 0.0F;
    cinder::vec3 _start_axis = cinder::vec3( 0, 0, 1 );
    cinder::vec3 _axis = cinder::vec3( 0, 0, 1 );
};
}
}