#pragma once
#include <treelike/scene.h>
#include <stack>
namespace treelike
{
class scene_manager
{
public:
    void replace( hardptr<scene> const& scene );
    void push_front( hardptr<scene> const& scene );
    void pop_front( );
    void push_back( hardptr<scene> const& scene );
    void pop_back( );
    size_t size( );
    bool empty( );
    hardptr<scene>& top( );
    void update( float delta );
    softptr<node> get_dont_destroy_node( );
private:
    std::vector<std::function<void( )>> _fn;
    std::list<hardptr<scene>> _stack;
    hardptr<node> _dont_destroy_node;
    float _delta;
public:
    static scene_manager* get_instans( );
    static void remove_instans( );
private:
    scene_manager( );
    static scene_manager* _instans;
};
}