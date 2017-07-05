#pragma once
#include <treelike/action/axis_to.h>
namespace treelike
{
namespace action
{
class axis_by : public axis_to
{
public:
    CREATE_H( axis_by, float duration, cinder::vec3 axis );
    bool init( float duration, cinder::vec3 axis );
private:
    void setup( ) override;
    void restart( ) override;
private:
    cinder::vec3 _init_axis = cinder::vec3( 0, 0, 1 );
};
}
}