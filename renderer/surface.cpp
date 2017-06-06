#include "surface.h"
#include "utility/assert_log.h"
#include "cinder/gl/gl.h"
using namespace cinder;
namespace renderer
{
CREATE_CPP( surface, cinder::vec2 size, cinder::ColorA color )
{
    CREATE( surface, size, color );
}
bool surface::init( cinder::vec2 size, cinder::ColorA color )
{
    _surface = Surface8u( size.x, size.y, true );
    for ( int y = 0; y < _surface.getHeight( ); ++y )
    {
        for ( int x = 0; x < _surface.getWidth( ); ++x )
        {
            _surface.setPixel( vec2( x, y ), color );
        }
    }
    _texture = gl::Texture::create( _surface );
    _content_size = _surface.getSize( );
    return true;
}
void surface::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
void surface::set_pixel( cinder::vec2 pixel, cinder::ColorA color )
{
    _surface.setPixel( pixel, color );

    glPixelStorei( GL_UNPACK_ROW_LENGTH, _surface.getHeight( ) );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, pixel.x );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, pixel.y );
    _texture->update( _surface.getData( ), GL_RGBA, GL_UNSIGNED_BYTE, 0, 1, 1, pixel );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
}
void surface::paint_fill_circle( cinder::vec2 pixel, float radius, cinder::ColorA color )
{
    pixel.y = _surface.getHeight( ) - pixel.y;

    if ( static_cast<int>( radius ) < 0 ) return;

    for ( int y = -radius; y <= radius; ++y )
    {
        for ( int x = -radius; x <= radius; ++x )
        {
            if ( radius < length( vec2( x, y ) ) ) continue;

            _surface.setPixel( ivec2( pixel ) + ivec2( x, y ), ColorA8u( color ) );
        }
    }

    ivec2 min( ivec2( pixel ) + ivec2( -radius - 1.0F, -radius - 1.0F ) );
    ivec2 max( ivec2( pixel ) + ivec2( radius + 1.0F, radius + 1.0F ) );
    min = glm::clamp( min, ivec2( 0 ), _surface.getSize( ) );
    max = glm::clamp( max, ivec2( 0 ), _surface.getSize( ) );

    glPixelStorei( GL_UNPACK_ROW_LENGTH, _surface.getWidth( ) );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, min.x );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, min.y );
    _texture->update( _surface.getData( ), GL_RGBA, GL_UNSIGNED_BYTE, 0, max.x - min.x, max.y - min.y, min );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
}
void surface::paint_fill_rect( cinder::Rectf rect, cinder::ColorA color )
{
    if ( rect.x2 < rect.x1 ) std::swap( rect.x1, rect.x2 );
    if ( rect.y2 < rect.y1 ) std::swap( rect.y1, rect.y2 );

    for ( int y = rect.y1; y <= rect.y2; ++y )
    {
        for ( int x = rect.x1; x <= rect.x2; ++x )
        {
            _surface.setPixel( ivec2( x, y ), ColorA8u( color ) );
        }
    }

    ivec2 min( ivec2( rect.x1, rect.y1 ) );
    ivec2 max( ivec2( rect.x2, rect.y2 ) );
    min = glm::clamp( min, ivec2( 0 ), _surface.getSize( ) );
    max = glm::clamp( max, ivec2( 0 ), _surface.getSize( ) );

    glPixelStorei( GL_UNPACK_ROW_LENGTH, _surface.getWidth( ) );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, min.x );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, min.y );
    _texture->update( _surface.getData( ), GL_RGBA, GL_UNSIGNED_BYTE, 0, max.x - min.x, max.y - min.y, min );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
}
cinder::ColorA surface::get_pixel( cinder::vec2 pixel )
{
    return _surface.getPixel( pixel );
}
}