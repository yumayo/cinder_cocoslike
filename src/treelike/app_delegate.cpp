#include <treelike/app_delegate.h>
#include <treelike/scene_manager.h>
#include <treelike/user_default.h>
#include <treelike/utility/assert_log.h>
#include <cinder/gl/gl.h>
namespace treelike
{
int const app_delegate::_INVALID_ID = -1;
void app_delegate::setup( )
{
    assert_log( !treelike::scene_manager::get_instans( )->empty( ), "シーンが代入されていません。", return );
    treelike::utility::log( "stand by ready!" );
}
void app_delegate::cleanup( )
{
    treelike::scene_manager::remove_instans( );
    treelike::user_default::remove_instans( );
}
void app_delegate::update( )
{
    auto elapsed_seconds = getElapsedSeconds( );
    auto delta = (float)elapsed_seconds - (float)_prev_second;
    _prev_second = elapsed_seconds;
    treelike::scene_manager::get_instans( )->top( )->_update( delta );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_update( delta );
    treelike::scene_manager::get_instans( )->update( delta );
}
void app_delegate::draw( )
{
    cinder::gl::clear( cinder::ColorA( 0.1F, 0.1F, 0.1F, 1.0F ) );
    treelike::scene_manager::get_instans( )->top( )->_render( cinder::mat3( ) );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_render( cinder::mat3( ) );
}
void app_delegate::mouseDown( cinder::app::MouseEvent event )
{
    treelike::scene_manager::get_instans( )->top( )->_mouse_began( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_mouse_began( event );
}
void app_delegate::mouseMove( cinder::app::MouseEvent event )
{
    treelike::scene_manager::get_instans( )->top( )->_mouse_moved( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_mouse_moved( event );
}
void app_delegate::mouseDrag( cinder::app::MouseEvent event )
{
    treelike::scene_manager::get_instans( )->top( )->_mouse_moved( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_mouse_moved( event );
}
void app_delegate::mouseUp( cinder::app::MouseEvent event )
{
    treelike::scene_manager::get_instans( )->top( )->_mouse_ended( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_mouse_ended( event );
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
        {
            treelike::scene_manager::get_instans( )->top( )->_touch_began( *itr );
            treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_touch_began( *itr );
        }
    }

    treelike::scene_manager::get_instans( )->top( )->_touches_began( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_touches_began( event );
}
void app_delegate::touchesMoved( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) )
    {
        treelike::scene_manager::get_instans( )->top( )->_touch_moved( *itr );
        treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_touch_moved( *itr );
    }

    treelike::scene_manager::get_instans( )->top( )->_touches_moved( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_touches_moved( event );
}
void app_delegate::touchesEnded( cinder::app::TouchEvent event )
{
    auto itr = std::find_if( std::begin( event.getTouches( ) ), std::end( event.getTouches( ) ), [ this ] ( cinder::app::TouchEvent::Touch& touch )
    {
        return touch.getId( ) == _touch_id;
    } );
    if ( itr != event.getTouches( ).end( ) )
    {
        treelike::scene_manager::get_instans( )->top( )->_touch_ended( *itr );
        treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_touch_ended( *itr );
        _touch_id = _INVALID_ID;
    }

    treelike::scene_manager::get_instans( )->top( )->_touches_ended( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_touches_ended( event );
}
void app_delegate::keyDown( cinder::app::KeyEvent event )
{
    treelike::scene_manager::get_instans( )->top( )->_key_down( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_key_down( event );
}
void app_delegate::keyUp( cinder::app::KeyEvent event )
{
    treelike::scene_manager::get_instans( )->top( )->_key_up( event );
    treelike::scene_manager::get_instans( )->get_dont_destroy_node( )->_key_up( event );
}
}