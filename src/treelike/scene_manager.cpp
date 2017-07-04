#include <treelike/scene_manager.h>
#include <treelike/utility/assert_log.h>
namespace treelike
{
scene_manager* scene_manager::_instans = nullptr;
void scene_manager::replace( hardptr<scene> const & scene )
{
    _fn.emplace_back( [ this, scene ]
    {
        auto& temp = _stack.begin( );
        _stack.push_front( scene );
        _stack.erase( temp );
        top( )->_update( _delta );
    } );
}
void scene_manager::push_front( hardptr<scene> const & scene )
{
    _fn.emplace_back( [ this, scene ] { _stack.push_front( scene ); } );
}
void scene_manager::pop_front( )
{
    assert_log( 1 <= _stack.size( ), "これ以上ポップできません。", return );
    _fn.emplace_back( [ this ] { _stack.pop_front( ); } );
}
void scene_manager::push_back( hardptr<scene> const & scene )
{
    _stack.push_back( scene );
}
void scene_manager::pop_back( )
{
    assert_log( 1 <= _stack.size( ), "これ以上ポップできません。", return );
    _stack.pop_back( );
}
size_t scene_manager::size( )
{
    return _stack.size( );
}
bool scene_manager::empty( )
{
    return _stack.empty( );
}
hardptr<scene>& scene_manager::top( )
{
    return _stack.front( );
}
void scene_manager::update( float delta )
{
    _delta = delta;
    for ( auto& fn : _fn )
    {
        fn( );
    }
    _fn.clear( );
}
softptr<node> scene_manager::get_dont_destroy_node( )
{
    return _root;
}
scene_manager * scene_manager::get_instans( )
{
    if ( !_instans ) _instans = new scene_manager;
    return _instans;
}
void scene_manager::remove_instans( )
{
    while ( !_instans->_stack.empty( ) )
    {
        _instans->_stack.front( )->remove_all_children( );
        _instans->_stack.pop_front( );
    }

    delete _instans;
    _instans = nullptr;
}
scene_manager::scene_manager( )
{
    _root = node::create( );
}

}