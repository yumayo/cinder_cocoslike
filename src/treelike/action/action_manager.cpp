#include <treelike/action/action_manager.h>
#include <algorithm>
#include <treelike/utility/assert_log.h>
#include <treelike/node.h>
namespace treelike
{
using namespace treelike::utility;
namespace action
{
void action_manager::add_action( hardptr<action> const& action, hardptr<node> const& target, bool pause )
{
    action->set_target( target );
    action->set_pause( pause );
    _actions.emplace_back( action );
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

void action_manager::remove_action( hardptr<action> const& act_weak )
{
    if ( _actions.empty( ) ) return;

    auto erase = std::remove_if( std::begin( _actions ), std::end( _actions ), [ this, act_weak ] ( hardptr<action>& act )
    {
        return act == act_weak;
    } );

    _actions.erase( erase, std::end( _actions ) );
}

void action_manager::remove_action_by_tag( int tag )
{
    assert_log( tag == node::INVALID_TAG, "無効なタグです。", return );

    auto act = this->get_action_by_tag( tag );

    if ( act )
    {
        this->remove_action( act );
    }
    else
    {
        log( "remove_action_by_tag(tag = %d): 子供が見つかりませんでした。", tag );
    }
}

void action_manager::remove_action_by_name( std::string const & name )
{
    assert_log( !name.empty( ), "無効な名前です。", return );

    auto act = this->get_action_by_name( name );

    if ( act )
    {
        this->remove_action( act );
    }
    else
    {
        log( "remove_action_by_name(name = %s): 子供が見つかりませんでした。", name.c_str( ) );
    }
}

bool action_manager::is_running( ) const
{
    return !_actions.empty( );
}

void action_manager::update( float delta )
{
    for ( auto const& obj : _actions ) // ここでエラってる時がある。
    {
        // managerまで溢れてくる値はシークエンス系以外なので無視して問題ありません。
        obj->update( delta );
    }
    auto erase = std::remove_if( std::begin( _actions ), std::end( _actions ), [ ] ( hardptr<action>& act )
    {
        return act->is_done( );
    } );

    _actions.erase( erase, std::end( _actions ) );

    for ( auto const& rem : _remove_signal ) rem( );
    _remove_signal.clear( );
}
}
}