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

// weak_ptr���g�p����Ƃ��ɁA�������� lock �Ƃ� expired ���g���̂����邢�̂Ń��b�s���O���܂��B

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
    // �|�C���^�̎��Ԃ�Ԃ��܂��B
    type& operator*( ) const noexcept
    {
        return *this->lock( );
    }
    // �|�C���^�̎��Ԃ�Ԃ��܂��B
    hard_pointer<type> operator->( ) const noexcept
    {
        return this->lock( );
    }
    // �|�C���^��null������؂�𔻒f���܂��B
    operator bool( ) const noexcept
    {
        return this->lock( ) && this->expired( );
    }
    // �|�C���^��null������؂�𔻒f��"not"��^���܂��B
    bool operator!( ) const noexcept
    {
        return !( this->lock( ) && this->expired( ) );
    }
    // ���ԓ��m�̔�r�����܂��B
    bool operator==( soft_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) == *right;
    }
    // ���ԓ��m�̔�r�����܂��B
    bool operator==( hard_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) == *right;
    }
    // ���ԓ��m�̔�r�����܂��B
    bool operator!=( soft_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) != *right;
    }
    // ���ԓ��m�̔�r�����܂��B
    bool operator!=( hard_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) != *right;
    }
    // ���ԓ��m�̔�r�����܂��B
    bool operator<( soft_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) < *right;
    }
    // ���ԓ��m�̔�r�����܂��B
    bool operator<( hard_pointer<type> const& right ) const noexcept
    {
        return *this->lock( ) < *right;
    }
    // �����I�Ƀ|�C���^�̎��Ԃ�Ԃ��܂��B
    operator type&( ) const noexcept
    {
        return *this->lock( );
    }
    // �����I�Ƀ|�C���^�̎��Ԃ�Ԃ��܂��B
    operator type const&( ) const noexcept
    {
        return *this->lock( );
    }
    // �����I�Ƀn�[�h�|�C���^�ɃL���X�g���܂��B
    operator hard_pointer<type>( ) const noexcept
    {
        return this->lock( );
    }
};
// ���ԓ��m�̔�r�����܂��B
template<class type>
bool operator==( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return *left == *right;
}
// ���ԓ��m�̔�r�����܂��B
template<class type>
bool operator!=( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return *left != *right;
}
// ���ԓ��m�̔�r�����܂��B
template<class type>
bool operator<( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return *left < *right;
}
template<class type>
using softptr = soft_pointer<type>;
}

}