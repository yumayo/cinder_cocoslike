#include <treelike/audio/base_player.h>
#include <treelike/utility/assert_log.h>
namespace treelike
{
namespace audio
{
base_player::~base_player( )
{
    _player_node->disconnectAll( );
}
void base_player::play( )
{
    _player_node->start( );
}
void base_player::stop( )
{
    _player_node->stop( );
    _player_node->disconnectAll( );
}
void base_player::set_gain( float value )
{
    _gain->setValue( value );
}
float base_player::get_gain( )
{
    return _gain->getValue( );
}
void base_player::set_loop( bool value )
{
    _player_node->setLoopEnabled( value );
}
bool base_player::get_loop( )
{
    return _player_node->isLoopEnabled( );
}
void base_player::set_loop_begin_second( float value )
{
    _player_node->setLoopBeginTime( value );
}
float base_player::get_loop_begin_second( )
{
    return _player_node->getLoopBeginTime( );
}
void base_player::set_loop_end_second( float value )
{
    _player_node->setLoopEndTime( value );
}
float base_player::set_loop_end_second( )
{
    return _player_node->getLoopEndTime( );
}
float base_player::get_current_play_second( )
{
    return _player_node->getReadPositionTime( );
}
float base_player::get_total_second( )
{
    return _player_node->getNumSeconds( );
}
}
}