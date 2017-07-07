#pragma once
#include <treelike/action/move_to.h>
namespace treelike
{
namespace action
{
class move_by : public move_to
{
public:
    CREATE_H( move_by, float duration, cinder::vec2 position );
    bool init( float duration, cinder::vec2 position );
    CREATE_H( move_by, float duration, cinder::vec3 position_3d );
    bool init( float duration, cinder::vec3 position_3d );
private:
    void setup( ) override;
    void restart( ) override;
private:
    cinder::vec3 _init_position;
};
}
}