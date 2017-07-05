#pragma once
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <cinder/app/App.h>
#include <cinder/Color.h>
#include <vector>
#include <string>
#include <treelike/forward.h>
#include <treelike/action/action_manager.h>
namespace treelike
{
class node : public std::enable_shared_from_this<node>
{
    friend class app_delegate;
    friend class scene_manager;
public:
    static const int INVALID_TAG = -1;
public:
    CREATE_H( node );
public:
    virtual ~node( );
public:
    virtual bool mouse_began( cinder::app::MouseEvent event ) { return false; }
    virtual void mouse_moved( cinder::app::MouseEvent event ) {}
    virtual void mouse_ended( cinder::app::MouseEvent event ) {}
    virtual bool touch_began( cinder::app::TouchEvent::Touch event ) { return false; }
    virtual void touch_moved( cinder::app::TouchEvent::Touch event ) {}
    virtual void touch_ended( cinder::app::TouchEvent::Touch event ) {}
    virtual void touches_began( cinder::app::TouchEvent event ) {}
    virtual void touches_moved( cinder::app::TouchEvent event ) {}
    virtual void touches_ended( cinder::app::TouchEvent event ) {}
    virtual void key_down( cinder::app::KeyEvent event ) {}
    virtual void key_up( cinder::app::KeyEvent event ) {}
    virtual void update( float delta ) {}
    virtual void render( ) {}
private:
    // à»â∫ÇÃä÷êîÇ≈ÉmÅ[Éhä‘ÇâÒÇµÇ‹Ç∑ÅB
    bool _mouse_began( cinder::app::MouseEvent event );
    bool _mouse_moved( cinder::app::MouseEvent event );
    bool _mouse_ended( cinder::app::MouseEvent event );
    bool _touch_began( cinder::app::TouchEvent::Touch event );
    bool _touch_moved( cinder::app::TouchEvent::Touch event );
    bool _touch_ended( cinder::app::TouchEvent::Touch event );
    void _touches_began( cinder::app::TouchEvent event );
    void _touches_moved( cinder::app::TouchEvent event );
    void _touches_ended( cinder::app::TouchEvent event );
    void _key_down( cinder::app::KeyEvent event );
    void _key_up( cinder::app::KeyEvent event );
    void _update( float delta );
    void _render( cinder::mat4 model_view_matrix );
protected:
    bool init( );
protected:
    bool _schedule_update = false;
public:
    virtual void set_schedule_update( bool const& value = true ) { _schedule_update = value; }
    inline bool const& get_schedule_update( ) const { return _schedule_update; }
protected:
    bool _block_schedule_update = false;
public:
    virtual void set_block_schedule_update( bool const& value = true ) { _block_schedule_update = value; }
    inline bool const& get_block_schedule_update( ) const { return _block_schedule_update; }
protected:
    bool _schedule_mouse_event = false;
public:
    virtual void set_schedule_mouse_event( bool const& value = true ) { _schedule_mouse_event = value; }
    inline bool const& get_schedule_mouse_event( ) const { return _schedule_mouse_event; }
protected:
    bool _schedule_touch_event = false;
public:
    virtual void set_schedule_touch_event( bool const& value = true ) { _schedule_touch_event = value; }
    inline bool const& get_schedule_touch_event( ) const { return _schedule_touch_event; }
protected:
    bool _schedule_touches_event = false;
public:
    virtual void set_schedule_touches_event( bool const& value = true ) { _schedule_touches_event = value; }
    inline bool const& get_schedule_touches_event( ) const { return _schedule_touches_event; }
protected:
    bool _schedule_key_event = false;
public:
    virtual void set_schedule_key_event( bool const& value = true ) { _schedule_key_event = value; }
    inline bool const& get_schedule_key_event( ) const { return _schedule_key_event; }
public:
    virtual void set_schedule_all( bool value = true );
protected:
    bool _block_schedule_event = false;
public:
    virtual void set_block_schedule_event( bool const& value = true ) { _block_schedule_event = value; }
    inline bool const& get_block_schedule_event( ) const { return _block_schedule_event; }
protected:
    cinder::vec2 _position = cinder::vec2( 0 );
    float _position_z = 0.0F;
public:
    virtual void set_position( cinder::vec2 const& value ) { _position = value; }
    inline cinder::vec2  const& get_position( ) const { return _position; }
    virtual void set_position_3d( cinder::vec3 const& value ) { memcpy( &_position, &value, sizeof( cinder::vec3 ) ); }
    inline cinder::vec3 const& get_position_3d( ) const { return *( cinder::vec3* )&_position; }
protected:
    cinder::vec2 _scale = cinder::vec2( 1 );
    float _scale_z = 0.0F;
public:
    virtual void set_scale( cinder::vec2 const& value ) { _scale = value; }
    inline cinder::vec2  const& get_scale( ) const { return _scale; }
    virtual void set_scale_3d( cinder::vec3 const& value ) { memcpy( &_scale, &value, sizeof( cinder::vec3 ) ); }
    inline cinder::vec3 const& get_scale_3d( ) const { return *( cinder::vec3* )&_scale; }
protected:
    cinder::vec2 _content_size = cinder::vec2( 0 );
    float _content_size_z = 0.0F;
public:
    virtual void set_content_size( cinder::vec2 const& value ) { _content_size = value; }
    inline cinder::vec2  const& get_content_size( ) const { return _content_size; }
    virtual void set_content_size_3d( cinder::vec3 const& value ) { memcpy( &_content_size, &value, sizeof( cinder::vec3 ) ); }
    inline cinder::vec3 const& get_content_size_3d( ) const { return *( cinder::vec3* )&_content_size; }
protected:
    float _rotation = 0.0F;
    cinder::vec3 _axis = cinder::vec3( 0, 0, 1 );
public:
    virtual void set_rotation( float const& value ) { _rotation = value; }
    inline float const& get_rotation( ) const { return _rotation; }
    virtual void set_axis( cinder::vec3 const& value ) { _axis = value; }
    inline cinder::vec3 const& get_axis( ) const { return _axis; }
protected:
    cinder::vec2 _anchor_point = cinder::vec2( 0 );
    float _anchor_point_z = 0.0F;
public:
    virtual void set_anchor_point( cinder::vec2 const& value ) { _anchor_point = value; }
    inline cinder::vec2  const& get_anchor_point( ) const { return _anchor_point; }
    virtual void set_anchor_point_3d( cinder::vec3 const& value ) { memcpy( &_anchor_point, &value, sizeof( cinder::vec3 ) ); }
    inline cinder::vec3 const& get_anchor_point_3d( ) const { return *( cinder::vec3* )&_anchor_point; }
protected:
    cinder::vec2 _pivot = cinder::vec2( 0 );
    float _pivot_z = 0.0F;
public:
    virtual void set_pivot( cinder::vec2 const& value ) { _pivot = value; }
    inline cinder::vec2  const& get_pivot( ) const { return _pivot; }
    virtual void set_pivot_3d( cinder::vec3 const& value ) { memcpy( &_pivot, &value, sizeof( cinder::vec3 ) ); }
    inline cinder::vec3 const& get_pivot_3d( ) const { return *( cinder::vec3* )&_pivot; }
protected:
    cinder::ColorA _color = cinder::ColorA::white( );
public:
    virtual void set_color( cinder::ColorA const& value ) { _color = value; }
    inline cinder::ColorA  const& get_color( ) const { return _color; }
    virtual void set_opacity( float alpha );
    float get_opacity( ) const;
protected:
    std::vector<hardptr<node>> _children;
public:
    std::vector<hardptr<node>> const& get_children( ) const;
protected:
    softptr<node> _parent;
public:
    virtual void set_parent( softptr<node> const& value );
    softptr<node> const& get_parent( ) const;
protected:
    int _tag = node::INVALID_TAG;
public:
    virtual void set_tag( int const& value ) { _tag = value; }
    inline int const& get_tag( ) const { return _tag; }
private:
    int _order = 0;
public:
    virtual void set_order( int const& value );
    int const& get_order( ) const;
protected:
    std::string _name = u8"";
public:
    virtual void set_name( std::string const& value );
    std::string const& get_name( ) const;
private:
    size_t _hash = 0;
protected:
    bool _running = true;
public:
    virtual void set_running( int const& value = true ) { _running = value; }
    inline bool const& get_running( ) const { return _running; }
protected:
    bool _visible = true;
public:
    virtual void set_visible( int const& value = true ) { _visible = value; }
    inline bool const& get_visible( ) const { return _visible; }
public:
    template<class ty>
    softptr<ty> add_child( hardptr<ty> const& value )
    {
        _update_end_signal.emplace_back( [ this, value ]
        {
            value->_parent = shared_from_this( );
            _children.emplace_back( value );
        } );
        return value;
    }
    softptr<node> get_child_by_name( std::string const& name )const;
    softptr<node> get_child_by_tag( int tag )const;
    void remove_child( softptr<node> const& child );
private:
    void remove_child_nonsafe( softptr<node> const& child );
public:
    void remove_child_by_name( std::string const& name );
    void remove_child_by_tag( int tag );
    void remove_all_children( );
    void remove_from_parent( );
private:
    bool _own_removing = false;
    std::vector<std::function<void( )>> _update_end_signal;
protected:
    bool _swallow = false;
public:
    softptr<node> get_root( );
    softptr<node> _get_root( );
protected:
    action::action_manager _action_manager;
public:
    void run_action( hardptr<action::action> const& action );
    softptr<action::action> get_action_by_name( std::string const& name )const;
    softptr<action::action> get_action_by_tag( int tag )const;
    void remove_all_actions( );
    void remove_action( softptr<action::action> const& action );
    void remove_action_by_tag( int tag );
    void remove_action_by_name( std::string const& name );
    bool is_running_action( ) const;
public:
    cinder::mat3 get_world_matrix( )const;
    cinder::mat4 get_world_matrix_3d( )const;
};
}