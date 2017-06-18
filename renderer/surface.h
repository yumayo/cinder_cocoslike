#pragma once
#include "node.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
namespace renderer
{
    class surface : public node
    {
    public:
        CREATE_H( surface, cinder::vec2 size, cinder::ColorA color = cinder::ColorA( 1, 1, 1, 1) );
        bool init( cinder::vec2 size, cinder::ColorA color );
        void render( ) override;
    public:
        void set_pixel( cinder::vec2 pixel, cinder::ColorA color );
        void paint_fill_circle( cinder::vec2 pixel, float radius, cinder::ColorA color );
        void paint_fill_rect( cinder::Rectf rect, cinder::ColorA color );
        cinder::ColorA get_pixel( cinder::vec2 pixel );
    protected:
        void texture_update( cinder::ivec2 min, cinder::ivec2 max );
        cinder::Surface8u _surface;
        cinder::gl::TextureRef _texture;
    };
}