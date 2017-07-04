#pragma once
#include <treelike/action/scale_to.h>
namespace treelike
{
namespace action
{
class scale_by : public scale_to
{
public:
    CREATE_H( scale_by, float duration, cinder::vec2 scale );
    bool init( float duration, cinder::vec2 scale );
private:
    void setup( ) override;
private:
    cinder::vec2 _init_scale;
};
}
}