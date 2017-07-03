#include "file_player.h"
#include "../utility/assert_log.h"
namespace audio
{
CREATE_CPP( file_player, std::string const& relative_path )
{
    CREATE( file_player, relative_path );
}
bool file_player::init( std::string const& relative_path )
{
    assert_log( !cinder::app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );
    auto ctx = cinder::audio::Context::master( );
    cinder::audio::SourceFileRef sourceFile = cinder::audio::load( cinder::app::loadAsset( relative_path ), ctx->getSampleRate( ) );
    _player_node = ctx->makeNode( new cinder::audio::FilePlayerNode( sourceFile ) );
    _gain = ctx->makeNode( new cinder::audio::GainNode( 1.0F ) );
    _player_node >> _gain >> ctx->getOutput( );
    ctx->enable( );
    return true;
}
}