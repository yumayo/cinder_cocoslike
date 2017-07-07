#pragma once
#include <mutex>
#include <map>
#include <thread>
namespace treelike
{
namespace utility
{
// 再帰的な呼び出し時にlockを掛けないmutexを管理できます。
// 主に、scoped_mutexと一緒に使います。
class recursion_usable_mutex
{
    std::mutex _mutex;
    std::mutex _own_ids;
    std::map<std::thread::id, int /*スレッドの番号の重複回数*/> _ids;
public:
    void lock( );
    void unlock( );
    std::pair<std::map<std::thread::id, int>::iterator, bool> emplace( );
    void erase( );
    int& get_duplication_count( );
};
}

}