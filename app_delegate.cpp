#include "app_delegate.h"
#include "scene_manager.h"
#include "user_default.h"
#include "utility/assert_log.h"
#include "cinder/gl/gl.h"
#include "../src/main.h"
int const app_delegate::_INVALID_ID = -1;
void app_delegate::setup( )
{
    utility::log( "stand by ready!" );
    entry_point( );

    auto elapsed_seconds = getElapsedSeconds( );
    auto delta = (float)elapsed_seconds - (float)_prev_second;
    _prev_second = elapsed_seconds;
    scene_manager::get_instans( )->update( delta );
}
void app_delegate::cleanup( )
{
    scene_manager::remove_instans( );
    user_default::remove_instans( );
}
void app_delegate::update( )
{
    auto elapsed_seconds = getElapsedSeconds( );
    auto delta = (float)elapsed_seconds - (float)_prev_second;
    _prev_second = elapsed_seconds;
    scene_manager::get_instans( )->top( )->_update( delta );
    scene_manager::get_instans( )->get_dont_destroy_node( ).lock( )->_update( delta );
    scene_manager::get_instans( )->update( delta );
}
void app_delegate::draw( )
{
    cinder::gl::clear( cinder::ColorA( 0.1, 0.1, 0.1, 1.0 ) );
    scene_manager::get_instans( )->top( )->_render( cinder::mat3( ) );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    scene_manager::get_instans( )->top( )->_mouse_began( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    scene_manager::get_instans( )->top( )->_mouse_moved( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    scene_manager::get_instans( )->top( )->_mouse_ended( event );
}
void app_delegate::touchesBegan( cinder::app::TouchEvent event )
{
    if ( _touch_id == _INVALID_ID )
    {
        _touch_id = event.getTouches( ).front( ).getId( );
        auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
        {
            return touch.getId( ) == _touch_id;
        } );
        if ( itr != event.getTouches( ).end( ) ) scene_manager::get_instans( )->top( )->_touch_began( *itr );
    }

    scene_manager::get_instans( )->top( )->_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) ) scene_manager::get_instans( )->top( )->_touch_moved( *itr );

    scene_manager::get_instans( )->top( )->_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) )
    {
        scene_manager::get_instans( )->top( )->_touch_ended( *itr );
        _touch_id = _INVALID_ID;
    }

    scene_manager::get_instans( )->top( )->_touches_ended( event );
}
void app_delegate::keyDown( cinder::app::KeyEvent event )
{
    scene_manager::get_instans( )->top( )->_key_down( event );
}
void app_delegate::keyUp( cinder::app::KeyEvent event )
{
    scene_manager::get_instans( )->top( )->_key_up( event );
}
