﻿#include <treelike/renderer/rect.h>
#include <cinder/gl/gl.h>
#include <treelike/utility/assert_log.h>
using namespace cinder;
namespace treelike
{
namespace renderer
{
CREATE_CPP( rect, cinder::vec2 const & size )
{
    CREATE( rect, size );
}
bool rect::init( cinder::vec2 const& size )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    set_content_size( size );
    return true;
}
void rect::render( )
{
    gl::drawSolidRect( Rectf( vec2( 0 ), _content_size ) );
}
}
}