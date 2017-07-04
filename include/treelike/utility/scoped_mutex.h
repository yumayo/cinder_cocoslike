#pragma once
#include <cinder/Noncopyable.h>
#include <treelike/utility/recursion_usable_mutex.h>
namespace treelike
{
namespace utility
{
// std::unique_lock<std::mutex>でも代用できるかと思ったのですが、
// lock(); lock(); ができなかったので、使用しないことにしました。
// スコープ内で別スレッドは作成できません。

// だめな例
//scoped_mutex s( /*recursion_usable_mutex*/ mutex );
//std::thread thread( [ ] 
//{
//    scoped_mutex s( /*recursion_usable_mutex*/ mutex );
//} );
//thread.join( );

// 良い例
//{
//    scoped_mutex s( /*recursion_usable_mutex*/ mutex );
//}
//std::thread thread( [ ] 
//{
//    scoped_mutex s( /*recursion_usable_mutex*/ mutex );
//} );
//thread.join( );
class scoped_mutex : cinder::Noncopyable
{
    recursion_usable_mutex& _mutex;
public:
    scoped_mutex( recursion_usable_mutex& mutex );
    ~scoped_mutex( );
};
}

}