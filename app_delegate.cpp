#include "app_delegate.h"
#include "scene_manager.h"
#include "user_default.h"
#include "utility/assert_log.h"
#include "cinder/gl/gl.h"
#define safe_top_scene if ( auto top = scene_manager::get_instans( )->top( ) ) top
int const app_delegate::_INVALID_ID = -1;
void app_delegate::setup( )
{
    utility::log( "stand by ready!" );
    app_delegate::entry_point( );
}
void app_delegate::cleanup( )
{
    scene_manager::remove_instans( );
    user_default::remove_instans( );
}
void app_delegate::update( )
{
    auto delta = (float)getElapsedSeconds( ) - (float)_prev_second;
    safe_top_scene->_update( delta );
    _prev_second = getElapsedSeconds( );
    scene_manager::get_instans( )->update( );
}
void app_delegate::draw( )
{
    cinder::gl::clear( cinder::ColorA( 0.1, 0.1, 0.1, 1.0 ) );
    safe_top_scene->_render( cinder::mat3( ) );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    safe_top_scene->_mouse_began( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    safe_top_scene->_mouse_moved( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    if ( isMultiTouchEnabled( ) ) return;
    safe_top_scene->_mouse_ended( event );
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
        if ( itr != event.getTouches( ).end( ) )
            safe_top_scene->_touch_began( *itr );
    }

    safe_top_scene->_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) )
        safe_top_scene->_touch_moved( *itr );

    safe_top_scene->_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) )
    {
        safe_top_scene->_touch_ended( *itr );
        _touch_id = _INVALID_ID;
    }

    safe_top_scene->_touches_ended( event );
}
#undef safe_top_scene