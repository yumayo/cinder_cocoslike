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
    hardptr<action> get_action_by_name( std::string const& name );
    hardptr<action> get_action_by_tag( int tag );
    void remove_all_actions( );
    void remove_action( hardptr<action> const& action );
    void remove_action_by_tag( int tag );
    void remove_action_by_name( std::string const& name );
    bool is_running( );
    void update( float delta );
private:
    std::list<hardptr<action>> _actions;
    std::vector<std::function<void( )>> _remove_signal;
};
}
}