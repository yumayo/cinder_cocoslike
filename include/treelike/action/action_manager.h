#pragma once
#include <treelike/action/action.h>
namespace treelike
{
namespace action
{
class action_manager
{
public:
    void add_action( hardptr<action> const& action, hardptr<node> const& target, bool pause );
    softptr<action> get_action_by_name( std::string const& name )const;
    softptr<action> get_action_by_tag( int tag )const;
    void remove_all_actions( );
    void remove_action( hardptr<action> const& remove_act );
private:
    void remove_action_nonsafe( hardptr<action> const& remove_act );
public:
    void remove_action_by_tag( int tag );
    void remove_action_by_name( std::string const& name );
    bool is_running( )const;
    void update( float delta );
private:
    std::vector<hardptr<action>> _actions;
    std::vector<std::function<void( )>> _update_end_signal;
};
}
}