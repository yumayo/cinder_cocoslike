﻿#include <treelike/action/scale_by.h>
#include <treelike/node.h>
using namespace cinder;
namespace treelike
{
namespace action
{
CREATE_CPP( scale_by, float duration, cinder::vec2 scale )
{
    CREATE( scale_by, duration, scale );
}
bool scale_by::init( float duration, cinder::vec2 scale )
{
    _duration = duration;
    _init_scale = vec3( scale, 1.0F );
    return true;
}
CREATE_CPP( scale_by, float duration, cinder::vec3 scale )
{
    CREATE( scale_by, duration, scale );
}
bool scale_by::init( float duration, cinder::vec3 scale )
{
    _duration = duration;
    _init_scale = scale;
    return true;
}
void scale_by::setup( )
{
    scale_to::setup( );
    _scale = _init_scale + _start_scale;
}
void scale_by::restart( )
{
    scale_to::restart( );
    setup( );
}
}
}