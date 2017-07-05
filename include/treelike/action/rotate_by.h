#pragma once
#include <treelike/action/rotate_to.h>
#include <cinder/Quaternion.h>
namespace treelike
{
namespace action
{
class rotate_by : public rotate_to
{
public:
    CREATE_H( rotate_by, float duration, float radian, cinder::vec3 axis = cinder::vec3( 0, 0, 1 ) );
    bool init( float duration, float radian, cinder::vec3 axis = cinder::vec3( 0, 0, 1 ) );
private:
    void setup( ) override;
    void restart( ) override;
private:
    float _init_rotation = 0.0F;
    cinder::vec3 _init_axis = cinder::vec3( 0, 0, 1 );
};
}
}