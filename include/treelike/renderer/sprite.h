#pragma once
#include <treelike/node.h>
#include <cinder/gl/Texture.h>
namespace treelike
{
namespace renderer
{
    class sprite : public node
    {
    public:
        CREATE_H( sprite, std::string const& relative_path );
        virtual ~sprite( ) { }
        bool init( std::string const& relative_path );
        virtual void render( ) override;
    protected:
        cinder::gl::TextureRef _texture;
    };
}
}