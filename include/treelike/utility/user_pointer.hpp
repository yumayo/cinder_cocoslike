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
class soft_pointer : public std::weak_ptr<type>
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
    template<class type2>
    soft_pointer( soft_pointer<type2> const& _Other ) noexcept
        : std::weak_ptr<type>( std::dynamic_pointer_cast<type>( _Other.lock( ) ) )
    {
    }
    template<class type2>
    soft_pointer( soft_pointer<type2>&& _Other ) noexcept
        : std::weak_ptr<type>( std::dynamic_pointer_cast<type>( _Other.lock( ) ) )
    {
    }
    template<class type2>
    soft_pointer( hard_pointer<type2> const& _Other ) noexcept
        : std::weak_ptr<type>( _Other )
    {
    }
    template<class type2>
    soft_pointer( hard_pointer<type2>&& _Other ) noexcept
        : std::weak_ptr<type>( std::move( _Other ) )
    {
    }
    template<class type2>
    soft_pointer& operator=( soft_pointer<type2> const& right ) noexcept
    {
        this->_Resetw( std::dynamic_pointer_cast<type>( right.lock( ) ) );
        return ( *this );
    }
    template<class type2>
    soft_pointer& operator=( hard_pointer<type2> const& right ) noexcept
    {
        this->_Resetw( std::dynamic_pointer_cast<type>( right ) );
        return ( *this );
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
        return this->lock( ) && ( !this->expired( ) );
    }
    // �|�C���^��null������؂�̔��f��"not"��^���܂��B
    bool operator!( ) const noexcept
    {
        return !( this->lock( ) && ( !this->expired( ) ) );
    }
    // �|�C���^���m�̔�r�����܂��B
    bool operator==( soft_pointer<type> const& right ) const noexcept
    {
        return this->get( ) == right.get( );
    }
    // �|�C���^���m�̔�r�����܂��B
    bool operator==( hard_pointer<type> const& right ) const noexcept
    {
        return this->get( ) == right.get( );
    }
    // �|�C���^���m�̔�r�����܂��B
    bool operator!=( soft_pointer<type> const& right ) const noexcept
    {
        return this->get( ) != right.get( );
    }
    // �|�C���^���m�̔�r�����܂��B
    bool operator!=( hard_pointer<type> const& right ) const noexcept
    {
        return this->get( ) != right.get( );
    }
    // �|�C���^���m�̔�r�����܂��B
    bool operator<( soft_pointer<type> const& right ) const noexcept
    {
        return this->get( ) < right.get( );
    }
    // �|�C���^���m�̔�r�����܂��B
    bool operator<( hard_pointer<type> const& right ) const noexcept
    {
        return this->get( ) < right.get( );
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
        return std::move( this->lock( ) );
    }
    // static_cast�����܂��B
    template<class type2>
    hard_pointer<type2> staticptr( ) const
    {
        return std::move( std::static_pointer_cast<type2>( this->lock( ) ) );
    }
    // dynamic_cast�����܂��B
    template<class type2>
    hard_pointer<type2> dynamicptr( ) const
    {
        return std::move( std::dynamic_pointer_cast<type2>( this->lock( ) ) );
    }
    // ���̃|�C���^�[��Ԃ��܂��B
    type* get( ) const noexcept
    {
        return this->lock( ).get( );
    }
};
// �|�C���^���m�̔�r�����܂��B
template<class type>
bool operator==( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return left.get( ) == right.get( );
}
// �|�C���^���m�̔�r�����܂��B
template<class type>
bool operator!=( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return left.get( ) != right.get( );
}
// �|�C���^���m�̔�r�����܂��B
template<class type>
bool operator<( hard_pointer<type> const& left, soft_pointer<type> const& right ) noexcept
{
    return left.get( ) < right.get( );
}
template<class type>
using softptr = soft_pointer<type>;
}
}