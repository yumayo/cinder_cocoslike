#include <treelike/renderer/surface_cubic.h>
#include <treelike/utility/assert_log.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace treelike
{
namespace renderer
{
CREATE_CPP( surface_cubic, cinder::vec2 size, cinder::ColorA color )
{
    CREATE( surface_cubic, size, color );
}
bool surface_cubic::init( cinder::vec2 size, cinder::ColorA color )
{
    _surface = Surface8u( size.x, size.y, true );
    for ( int y = 0; y < _surface.getHeight( ); ++y )
    {
        for ( int x = 0; x < _surface.getWidth( ); ++x )
        {
            _surface.setPixel( vec2( x, y ), color );
        }
    }
    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    _texture = gl::Texture::create( _surface, format );
    _content_size = _surface.getSize( );
    return true;
}
void surface_cubic::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
void surface_cubic::set_pixel( cinder::vec2 pixel, cinder::ColorA color )
{
    _surface.setPixel( pixel, color );
    texture_update( pixel, ivec2( pixel ) + 1 );
}
void surface_cubic::paint_fill_circle( cinder::vec2 pixel, float radius, cinder::ColorA color )
{
    if ( radius < 0 ) return;

    Rectf rect( glm::floor( pixel - radius - 1.0F ), glm::ceil( pixel + radius ) );

    for ( int y = rect.y1; y <= rect.y2; ++y )
    {
        for ( int x = rect.x1; x <= rect.x2; ++x )
        {
            if ( radius < length( vec2( x, y ) - rect.getCenter( ) ) ) continue;
            if ( x < 0 || y < 0 || x >= _surface.getWidth( ) || y >= _surface.getHeight( ) ) continue;
            _surface.setPixel( ivec2( x, y ), ColorA8u( color ) );
        }
    }

    ivec2 min( ivec2( rect.x1, rect.y1 ) );
    min = glm::clamp( min, ivec2( 0 ), _surface.getSize( ) );
    ivec2 max( ivec2( rect.x2, rect.y2 ) );
    max = glm::clamp( max, ivec2( 0 ), _surface.getSize( ) );

    texture_update( min, max );
}
void surface_cubic::paint_fill_rect( cinder::Rectf rect, cinder::ColorA color )
{
    if ( rect.x2 < rect.x1 ) std::swap( rect.x1, rect.x2 );
    if ( rect.y2 < rect.y1 ) std::swap( rect.y1, rect.y2 );

    rect.x1 = glm::floor( rect.x1 );
    rect.y1 = glm::floor( rect.y1 );
    rect.x2 = glm::ceil( rect.x2 );
    rect.y2 = glm::ceil( rect.y2 );

    for ( int y = rect.y1; y <= rect.y2; ++y )
    {
        for ( int x = rect.x1; x <= rect.x2; ++x )
        {
            _surface.setPixel( ivec2( x, y ), ColorA8u( color ) );
        }
    }

    ivec2 min( ivec2( rect.x1, rect.y1 ) );
    min = glm::clamp( min, ivec2( 0 ), _surface.getSize( ) );
    ivec2 max( ivec2( rect.x2, rect.y2 ) );
    max = glm::clamp( max, ivec2( 0 ), _surface.getSize( ) );

    texture_update( min, max );
}
cinder::ColorA surface_cubic::get_pixel( cinder::vec2 pixel )
{
    return _surface.getPixel( pixel );
}
void surface_cubic::texture_update( cinder::ivec2 min, cinder::ivec2 max )
{
    auto width = max.x - min.x;
    auto height = max.y - min.y;

    std::unique_ptr<unsigned char [ ]> data( new unsigned char[sizeof( unsigned char ) * 4 * width * height] );

    for ( int y = min.y; y < max.y; ++y )
    {
        for ( int x = min.x; x < max.x; ++x )
        {
            auto col = _surface.getPixel( ivec2( x, y ) );
            ivec2 pos = ivec2( x, y ) - min;
            data[( height - 1 - pos.y ) * 4 * width + pos.x * 4 + 0] = col.r;
            data[( height - 1 - pos.y ) * 4 * width + pos.x * 4 + 1] = col.g;
            data[( height - 1 - pos.y ) * 4 * width + pos.x * 4 + 2] = col.b;
            data[( height - 1 - pos.y ) * 4 * width + pos.x * 4 + 3] = col.a;
        }
    }

    glPixelStorei( GL_UNPACK_ROW_LENGTH, width );
    _texture->update( data.get( ), GL_RGBA, GL_UNSIGNED_BYTE, 0, width, height, ivec2( min.x, _surface.getHeight( ) - min.y - height ) );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
}
}
}