#include <treelike/action/action_manager.h>
#include <algorithm>
#include <treelike/utility/assert_log.h>
#include <treelike/node.h>
namespace treelike
{
using namespace treelike::utility;
namespace action
{
void action_manager::add_action( hardptr<action> act, hardptr<node> target, bool pause )
{
    act->set_target( target );
    act->set_pause( pause );
    _update_end_signal.emplace_back( [ this, act ]
    {
        _actions.emplace_back( act );
    } );
}
softptr<action> action_manager::get_action_by_name( std::string const & name ) const
{
    assert_log( !name.empty( ), "無効な名前です。", return nullptr );

    std::hash<std::string> h;
    size_t hash = h( name );

    auto itr = std::find_if( std::cbegin( _actions ), std::cend( _actions ), [ this, hash, name ] ( hardptr<action> const& act )
    {
        return act->_hash == hash && act->_name.compare( name ) == 0;
    } );

    if ( itr != std::cend( _actions ) )
    {
        return *itr;
    }
    return nullptr;
}
softptr<action> action_manager::get_action_by_tag( int tag ) const
{
    assert_log( tag == node::INVALID_TAG, "無効なタグです。", return nullptr );

    auto itr = std::find_if( std::cbegin( _actions ), std::cend( _actions ), [ this, tag ] ( hardptr<action> const& act )
    {
        return act->_tag == tag;
    } );

    if ( itr != std::cend( _actions ) )
    {
        return *itr;
    }
    return nullptr;
}
void action_manager::remove_all_actions( )
{
    _actions.clear( );
}
void action_manager::remove_action( softptr<action> remove_act )
{
    if ( _actions.empty( ) ) return;
    _update_end_signal.emplace_back( [ this, remove_act ]
    {
        auto erase = std::remove_if( std::begin( _actions ), std::end( _actions ), [ this, remove_act ] ( hardptr<action>& act )
        {
            return act == remove_act;
        } );
        _actions.erase( erase, std::end( _actions ) );
    } );
}
void action_manager::remove_action_nonsafe( softptr<action> remove_act )
{
    if ( _actions.empty( ) ) return;
    auto erase = std::remove_if( std::begin( _actions ), std::end( _actions ), [ this, remove_act ] ( hardptr<action>& act )
    {
        return act == remove_act;
    } );
    _actions.erase( erase, std::end( _actions ) );
}
void action_manager::remove_action_by_tag( int tag )
{
    assert_log( tag == node::INVALID_TAG, "無効なタグです。", return );

    _update_end_signal.emplace_back( [ this, tag ]
    {
        if ( auto act = this->get_action_by_tag( tag ) )
        {
            remove_action_nonsafe( act );
        }
        else
        {
            log( "remove_action_by_tag(tag = %d): 子供が見つかりませんでした。", tag );
        }
    } );
}
void action_manager::remove_action_by_name( std::string const & name )
{
    assert_log( !name.empty( ), "無効な名前です。", return );

    _update_end_signal.emplace_back( [ this, name ]
    {
        if ( auto act = this->get_action_by_name( name ) )
        {
            remove_action_nonsafe( act );
        }
        else
        {
            log( "remove_action_by_name(name = %s): 子供が見つかりませんでした。", name.c_str( ) );
        }
    } );
}
bool action_manager::is_running( ) const
{
    return !_actions.empty( );
}
void action_manager::update( float delta )
{
    for ( auto const& obj : _actions )
    {
        // managerまで溢れてくる値はシークエンス系以外なので無視して問題ありません。
        obj->update( delta );
    }
    
    for ( auto const& rem : _update_end_signal ) rem( );
    _update_end_signal.clear( );

    auto erase = std::remove_if( std::begin( _actions ), std::end( _actions ), [ ] ( hardptr<action>& act )
    {
        return act->is_done( );
    } );
    _actions.erase( erase, std::end( _actions ) );
}
}
}