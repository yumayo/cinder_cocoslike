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
private:
    void setup( ) override;
private:
    cinder::vec2 _init_position;
};
}
}