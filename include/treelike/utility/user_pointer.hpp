#pragma once
#include <memory>
namespace treelike
{
namespace utility
{
template<class type>
using hard_pointer = std::shared_ptr<type>;
template<class type>
using hardptr = hard_pointer<type>;

// weak_ptrを使用するときに、いちいち lock とか expired を使うのがだるいのでラッピングします。

template<class type>
class soft_pointer : protected std::weak_ptr<type>
{
public:
    soft_pointer( ) = default;
    soft_pointer( hard_pointer<type> const& right ) noexcept
        : std::weak_ptr<type>( right )
    {
    }
    soft_pointer( hard_pointer<type>&& right ) noexcept
        : std::weak_ptr<type>( std::move( right ) )
    {
    }
    // ポインタの実態を返します。
    type& operator*( ) const noexcept
    {
        return *this->lock( );
    }
    // ポインタの実態を返します。
    hard_pointer<type> operator->( ) const noexcept
    {
        return this->lock( );
    }
    // ポインタがnullや期限切れを判断します。
    operator bool( ) const noexcept
    {
        return this->lock( ) && this->expired( );
    }
    // ポインタにnullや期限切れを判断の"not"を与えます。
    bool operator!( ) const noexcept
    {
        return !( this->lock( ) && this->expired( ) );
    }
    // 実態同士の比較をします。
    bool operator==( soft_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) == *right;
    }
    // 実態同士の比較をします。
    bool operator==( hard_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) == *right;
    }
    // 実態同士の比較をします。
    bool operator!=( soft_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) != *right;
    }
    // 実態同士の比較をします。
    bool operator!=( hard_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) != *right;
    }
    // 実態同士の比較をします。
    bool operator<( soft_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) < *right;
    }
    // 実態同士の比較をします。
    bool operator<( hard_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) < *right;
    }
    // 自動的にポインタの実態を返します。
    operator type&( ) const noexcept
    {
        return *this->lock( );
    }
    // 自動的にポインタの実態を返します。
    operator type const&( ) const noexcept
    {
        return *this->lock( );
    }
    // 自動的にハードポインタにキャストします。
    operator hard_pointer<type>( ) const noexcept
    {
        return this->lock( );
    }
};
// 実態同士の比較をします。
template<class type>
bool operator==( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return *left == *right;
}
// 実態同士の比較をします。
template<class type>
bool operator!=( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return *left != *right;
}
// 実態同士の比較をします。
template<class type>
bool operator<( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return *left < *right;
}
template<class type>
using softptr = soft_pointer<type>;
}

}