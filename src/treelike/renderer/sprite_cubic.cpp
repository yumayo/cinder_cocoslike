#include <treelike/renderer/sprite_cubic.h>
#include <treelike/utility/assert_log.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace treelike
{
namespace renderer
{
CREATE_CPP( sprite_cubic, std::string const& relative_path )
{
    CREATE( sprite_cubic, relative_path );
}
bool sprite_cubic::init( std::string const& relative_path )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );

    assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    _texture = gl::Texture::create( loadImage( app::loadAsset( relative_path ) ), format );
    _content_size = _texture->getSize( );

    return true;
}
}
}