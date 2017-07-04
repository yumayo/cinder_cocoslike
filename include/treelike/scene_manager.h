#pragma once
#include <treelike/scene.h>
#include <stack>
namespace treelike
{
class scene_manager
{
public:
    void replace( std::shared_ptr<scene> const& scene );
    void push_front( std::shared_ptr<scene> const& scene );
    void pop_front( );
    void push_back( std::shared_ptr<scene> const& scene );
    void pop_back( );
    size_t size( );
    bool empty( );
    std::shared_ptr<scene>& top( );
    void update( float delta );
    std::weak_ptr<node> get_dont_destroy_node( );
private:
    std::vector<std::function<void( )>> _fn;
    std::list<std::shared_ptr<scene>> _stack;
    std::shared_ptr<node> _root;
    float _delta;
public:
    static scene_manager* get_instans( );
    static void remove_instans( );
private:
    scene_manager( );
    static scene_manager* _instans;
};
}