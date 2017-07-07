﻿#include <treelike/node.h>
#include <cinder/gl/gl.h>
#include <treelike/utility/assert_log.h>
#include <treelike/utility/collision.h>
namespace treelike
{
using namespace cinder;
using namespace treelike::utility;
class exception_remove_self : public std::runtime_error
{
public:
    exception_remove_self( )
        : std::runtime_error( "自分自身が削除されました。" )
    {   // construct from message string
    }
};
CREATE_CPP( node )
{
    CREATE( node );
}
node::~node( )
{
    if ( !_name.empty( ) ) log( "Destroy node: [%s]", _name.c_str( ) );
}
bool node::_mouse_began( cinder::app::MouseEvent event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                // 子供がモーダルオブジェクトだった場合
                if ( _children[_riterator]->_mouse_began( event ) )
                {
                    return true;
                }
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_mouse_event )
    {
        if ( mouse_began( event ) )
        {
            _swallow = true;
            return true;
        }
    }
    return false;
}
bool node::_mouse_moved( cinder::app::MouseEvent event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                if ( _children[_riterator]->_mouse_moved( event ) )
                {
                    return true;
                }
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_mouse_event )
    {
        if ( _swallow )
        {
            mouse_moved( event );
            return true;
        }
    }
    return false;
}
bool node::_mouse_ended( cinder::app::MouseEvent event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                if ( _children[_riterator]->_mouse_ended( event ) )
                {
                    return true;
                }
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_mouse_event )
    {
        if ( _swallow )
        {
            mouse_ended( event );
            _swallow = false;
            return true;
        }
    }
    return false;
}
bool node::_touch_began( cinder::app::TouchEvent::Touch event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                // 子供がモーダルオブジェクトだった場合
                if ( _children[_riterator]->_touch_began( event ) )
                {
                    return true;
                }
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_touch_event )
    {
        if ( touch_began( event ) )
        {
            _swallow = true;
            return true;
        }
    }
    return false;
}
bool node::_touch_moved( cinder::app::TouchEvent::Touch event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                if ( _children[_riterator]->_touch_moved( event ) )
                {
                    return true;
                }
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_touch_event )
    {
        if ( _swallow )
        {
            touch_moved( event );
            return true;
        }
    }
    return false;
}
bool node::_touch_ended( cinder::app::TouchEvent::Touch event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                if ( _children[_riterator]->_touch_ended( event ) )
                {
                    return true;
                }
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_touch_event )
    {
        if ( _swallow )
        {
            touch_ended( event );
            _swallow = false;
            return true;
        }
    }
    return false;
}
void node::_touches_began( cinder::app::TouchEvent event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                _children[_riterator]->_touches_began( event );
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_touches_event )
    {
        touches_began( event );
    }
}
void node::_touches_moved( cinder::app::TouchEvent event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                _children[_riterator]->_touches_moved( event );
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_touches_event )
    {
        touches_moved( event );
    }
}
void node::_touches_ended( cinder::app::TouchEvent event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                _children[_riterator]->_touches_ended( event );
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_touches_event )
    {
        touches_ended( event );
    }
}
void node::_key_down( cinder::app::KeyEvent event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            try
            {
                _children[_riterator]->_key_down( event );
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    if ( _schedule_key_event )
    {
        key_down( event );
    }
}
void node::_key_up( cinder::app::KeyEvent event )
{
    if ( !_block_schedule_event )
    {
        _iterator_direction = true;
        for ( _riterator = _children.size( ) - 1; _riterator >= 0; --_riterator )
        {
            _children[_riterator]->_key_up( event );
        }
    }

    if ( _schedule_key_event )
    {
        key_up( event );
    }
}
void node::_update( float delta )
{
    if ( !_block_schedule_update )
    {
        _iterator_direction = false;
        for ( _iterator = 0; _iterator < _children.size( ); ++_iterator )
        {
            try
            {
                _children[_iterator]->_update( delta );
            }
            catch ( exception_remove_self const& e )
            {
                log( e.what( ) );
            }
        }
    }
    _action_manager.update( delta );
    if ( _schedule_update ) update( delta );

    for ( auto const& signal : _update_end_signal ) signal( );
    _update_end_signal.clear( );
}
void node::_render( cinder::mat4 m )
{
    m = translate( m, get_position_3d( ) );
    m = scale( m, get_scale_3d( ) );
    m = rotate( m, get_rotation( ), get_axis( ) );
    m = translate( m, -get_content_size_3d( ) * get_anchor_point_3d( ) );
    gl::setModelMatrix( m );
    if ( _visible )
    {
        //std::pair<vec2, vec2> aabb;
        //if ( utility::hit_window_aabb( m, shared_from_this( ), &aabb ) )
        {
            gl::color( _color );
            this->render( );
        }
        //#ifdef _DEBUG
        //gl::setModelMatrix( mat4( ) );
        //gl::color( Color::white( ) );
        //gl::drawStrokedRect( Rectf( aabb.first, aabb.second ) );
        //#endif
    }
    m = translate( m, get_content_size_3d( ) * get_pivot_3d( ) );
    _iterator_direction = false;
    for ( _iterator = 0; _iterator < _children.size( ); ++_iterator )
    {
        try
        {
            _children[_iterator]->_render( m );
        }
        catch ( exception_remove_self const& e )
        {
            log( e.what( ) );
        }
    }
}
bool node::init( )
{
    return true;
}
void node::set_schedule_all( bool value )
{
    _schedule_update = value;
    _schedule_mouse_event = value;
    _schedule_touch_event = value;
    _schedule_touches_event = value;
    _schedule_key_event = value;
}
void node::set_opacity( float alpha )
{
    _color.a = alpha;
}
float node::get_opacity( ) const
{
    return _color.a;
}
std::vector<hardptr<node>> const& node::get_children( ) const
{
    return _children;
}
void node::sort_children( std::function<bool( hardptr<node>&a, hardptr<node>&b )> const & func )
{
    std::sort( _children.begin( ), _children.end( ), func );
}
void node::set_parent( softptr<node> value )
{
    assert_log( _parent, "親が未定義です。", return );
    softptr<node> prev_parent = _parent;
    value->add_child( shared_from_this( ) );
    prev_parent->remove_child( shared_from_this( ) );
}
softptr<node> const& node::get_parent( ) const
{
    return _parent;
}
void node::set_order( int const& value )
{
    _order = value;
    _update_end_signal.emplace_back( [ this, value ]
    {
        std::sort( _children.begin( ), _children.end( ), [ ] ( hardptr<node>& a, hardptr<node>& b )
        {
            return a->_order < b->_order;
        } );
    } );
}
int const& node::get_order( ) const
{
    return _order;
}
void node::set_name( std::string const& value )
{
    _name = value;
    std::hash<std::string> h;
    _hash = h( value );
}
std::string const& node::get_name( ) const
{
    return _name;
}
softptr<node> node::get_child_by_name( std::string const & name ) const
{
    assert_log( !name.empty( ), "無効な名前です。", return nullptr );
    std::hash<std::string> h;
    size_t hash = h( name );
    auto itr = std::find_if( std::cbegin( _children ), std::cend( _children ), [ this, hash, name ] ( hardptr<node> const& child )
    {
        return child->_hash == hash && child->_name.compare( name ) == 0;
    } );
    if ( itr != std::cend( _children ) )
    {
        return *itr;
    }
    return nullptr;
}
softptr<node> node::get_child_by_tag( int tag ) const
{
    assert_log( tag != node::INVALID_TAG, "無効なタグです。", return nullptr );
    auto itr = std::find_if( std::cbegin( _children ), std::cend( _children ), [ this, tag ] ( hardptr<node> const& n )
    {
        return n->_tag == tag;
    } );

    if ( itr != std::cend( _children ) )
    {
        return *itr;
    }
    return nullptr;
}
void node::remove_child( softptr<node> child )
{
    if ( _children.empty( ) ) return;
    auto erase_itr = std::find_if( std::begin( _children ), std::end( _children ), [ this, child ] ( hardptr<node>& n )
    {
        return n == child;
    } );
    auto index = std::distance( std::begin( _children ), erase_itr );
    if ( !_iterator_direction ) // コピペしまくってるのは、自分自身を削除した後にthisにアクセスしてしまうと、エラーになる可能性が高いので。
    {
        if ( index == _iterator ) { _children.erase( std::begin( _children ) + index ); _iterator--; throw exception_remove_self( ); }
        else { _children.erase( std::begin( _children ) + index ); if ( index < _iterator ) _iterator--; }
    }
    else
    {
        if ( index == _riterator ) { _children.erase( std::begin( _children ) + index ); _riterator--; throw exception_remove_self( ); }
        else { _children.erase( std::begin( _children ) + index ); if ( index < _riterator ) _riterator--; }
    }
}
void node::remove_child_by_name( std::string const & name )
{
    assert_log( !name.empty( ), "無効な名前です。", return );
    if ( auto child = this->get_child_by_name( name ) )
    {
        remove_child( child );
    }
    else
    {
        log( "remove_child_by_name(name = %s): 子供が見つかりませんでした。", name.c_str( ) );
    }
}
void node::remove_child_by_tag( int tag )
{
    assert_log( tag != node::INVALID_TAG, "無効なタグです。", return );
    if ( auto child = this->get_child_by_tag( tag ) )
    {
        remove_child( child );
    }
    else
    {
        log( "remove_child_by_tag(tag = %d): 子供が見つかりませんでした。", tag );
    }
}
void node::remove_all_children( )
{
    _children.clear( );
}
void node::remove_from_parent( )
{
    assert_log( _parent, "親が未定義です。", return );
    _parent->remove_child( shared_from_this( ) );
}
softptr<node> node::get_root( )
{
    return _get_root( );
}
softptr<node> node::_get_root( )
{
    if ( _parent )
    {
        return _parent->_get_root( );
    }
    else
    {
        return shared_from_this( );
    }
}
void node::run_action( hardptr<action::action> action )
{
    _action_manager.add_action( action, shared_from_this( ), !_running );
    action->setup( );
}
softptr<action::action> node::get_action_by_name( std::string const & name ) const
{
    return _action_manager.get_action_by_name( name );
}
softptr<action::action> node::get_action_by_tag( int tag ) const
{
    return _action_manager.get_action_by_tag( tag );
}
void node::remove_all_actions( )
{
    _action_manager.remove_all_actions( );
}
void node::remove_action( softptr<action::action> action )
{
    _action_manager.remove_action( action );
}
void node::remove_action_by_tag( int tag )
{
    _action_manager.remove_action_by_tag( tag );
}
void node::remove_action_by_name( std::string const & name )
{
    _action_manager.remove_action_by_name( name );
}
bool node::is_running_action( ) const
{
    return _action_manager.is_running( );
}
cinder::mat3 node::get_world_matrix( ) const
{
    std::vector<mat3> mats;
    auto p = _parent;
    while ( p )
    {
        auto m = translate( mat3( ), p->get_position( ) );
        m = scale( m, p->get_scale( ) );
        m = rotate( m, p->get_rotation( ) );
        m = translate( m, -p->get_content_size( ) * p->get_anchor_point( ) );
        m = translate( m, p->get_content_size( ) * p->get_pivot( ) );
        mats.emplace_back( std::move( m ) );
        p = p->_parent;
    }
    mat3 result;
    for ( auto itr = mats.rbegin( ); itr != mats.rend( ); ++itr )
    {
        result *= *itr;
    }
    return result;
}
cinder::mat4 node::get_world_matrix_3d( ) const
{
    std::vector<mat4> mats;
    auto p = _parent;
    while ( p )
    {
        auto m = translate( mat4( ), p->get_position_3d( ) );
        m = scale( m, p->get_scale_3d( ) );
        m = rotate( m, get_rotation( ), get_axis( ) );
        m = translate( m, -p->get_content_size_3d( ) * p->get_anchor_point_3d( ) );
        m = translate( m, p->get_content_size_3d( ) * p->get_pivot_3d( ) );
        mats.emplace_back( std::move( m ) );
        p = p->_parent;
    }
    mat4 result;
    for ( auto itr = mats.rbegin( ); itr != mats.rend( ); ++itr )
    {
        result *= *itr;
    }
    return result;
}
}