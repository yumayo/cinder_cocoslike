#pragma once
#include <treelike/node.h>
#include <cinder/audio/audio.h>
namespace treelike
{
namespace audio
{
class base_player : public node
{
public:
    virtual ~base_player( );
    virtual bool init( std::string const& relative_path ) = 0;
    void play( );
    void stop( );
    void set_gain( float value );
    float get_gain( );
    void set_loop( bool value );
    bool get_loop( );
    void set_loop_begin_second( double value );
    double get_loop_begin_second( );
    void set_loop_end_second( double value );
    double set_loop_end_second( );
    double get_current_play_second( );
    double get_total_second( );
protected:
    cinder::audio::GainNodeRef _gain;
    cinder::audio::SamplePlayerNodeRef _player_node;
};
}
}