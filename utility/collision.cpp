#include "collision.h"
using namespace cinder;
namespace utility
{
class segment
{
public:
    segment( cinder::vec2 start, cinder::vec2 end );
public:
    cinder::vec2 start;
    cinder::vec2 end;
};
segment::segment( cinder::vec2 start, cinder::vec2 end )
    : start( start )
    , end( end )
{ }
bool hit_point_polygon_2d( cinder::vec2 point, cinder::vec2 a, cinder::vec2 b, cinder::vec2 c )
{
    vec2 AB = b - a;
    vec2 BP = point - b;

    vec2 BC = c - b;
    vec2 CP = point - c;

    vec2 CA = a - c;
    vec2 AP = point - a;

    double c1 = determinant_2d( AB, BP );
    double c2 = determinant_2d( BC, CP );
    double c3 = determinant_2d( CA, AP );

    return c1 >= 0.0 && c2 >= 0.0 && c3 >= 0.0;
}
bool hit_point_plane_2d( cinder::vec2 point, cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d )
{
    vec2 AB = b - a;
    vec2 BP = point - b;

    vec2 BC = c - b;
    vec2 CP = point - c;

    vec2 CD = d - c;
    vec2 DP = point - d;

    vec2 DA = a - d;
    vec2 AP = point - a;

    double c1 = determinant_2d( AB, BP );
    double c2 = determinant_2d( BC, CP );
    double c3 = determinant_2d( CD, DP );
    double c4 = determinant_2d( DA, AP );

    return c1 >= 0.0 && c2 >= 0.0 && c3 >= 0.0 && c4 >= 0.0;
}
bool hit_point_plane_2d( cinder::vec2 point, std::shared_ptr<node> const & object )
{
    auto mat = object->get_world_matrix( );
    auto _content_size = object->get_content_size( );
    auto _anchor_point = object->get_anchor_point( );
    auto _position = object->get_position( );
    auto _scale = object->get_scale( );
    auto _rotation = object->get_rotation( );

    auto obj = mat;
    obj = translate( obj, _position );
    obj = scale( obj, _scale );
    obj = rotate( obj, _rotation );
    obj = translate( obj, -_content_size * _anchor_point );

    auto ma = translate( obj, vec2( 0.0F, 0.0F ) );
    auto a = vec2( ma[2][0], ma[2][1] );
    auto mb = translate( obj, vec2( _content_size.x, 0.0F ) );
    auto b = vec2( mb[2][0], mb[2][1] );
    auto mc = translate( obj, vec2( _content_size.x, _content_size.y ) );
    auto c = vec2( mc[2][0], mc[2][1] );
    auto md = translate( obj, vec2( 0.0F, _content_size.y ) );
    auto d = vec2( md[2][0], md[2][1] );

    return utility::hit_point_plane_2d( point, a, b, c, d );
}
bool hit_point_plane_2d_default_size( cinder::vec2 point, std::shared_ptr<node> const & object )
{
    auto mat = object->get_world_matrix( );
    auto _content_size = object->get_content_size( );
    auto _anchor_point = object->get_anchor_point( );
    auto _position = object->get_position( );
    auto _scale = object->get_scale( );
    auto _rotation = object->get_rotation( );

    auto obj = mat;
    obj = translate( obj, _position );
    //obj = scale( obj, _scale ); ボタンの拡大アクション時にスケールごと当たり判定を起こしたくないので。
    obj = rotate( obj, _rotation );
    obj = translate( obj, -_content_size * _anchor_point );

    auto ma = translate( obj, vec2( 0.0F, 0.0F ) );
    auto a = vec2( ma[2][0], ma[2][1] );
    auto mb = translate( obj, vec2( _content_size.x, 0.0F ) );
    auto b = vec2( mb[2][0], mb[2][1] );
    auto mc = translate( obj, vec2( _content_size.x, _content_size.y ) );
    auto c = vec2( mc[2][0], mc[2][1] );
    auto md = translate( obj, vec2( 0.0F, _content_size.y ) );
    auto d = vec2( md[2][0], md[2][1] );

    return utility::hit_point_plane_2d( point, a, b, c, d );
}
bool hit_segment( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 )
{
    segment seg1( s1, e1 );
    segment seg2( s2, e2 );

    const vec2 sub_start = seg1.start - seg2.start;
    const vec2 sub_seg1 = seg1.end - seg1.start;
    const vec2 sub_seg2 = seg2.end - seg2.start;

    const double seg1_t = determinant_2d( sub_seg2, sub_start ) / determinant_2d( sub_seg1, sub_seg2 );
    if ( seg1_t < 0 || 1 < seg1_t ) return false;

    const double seg2_t = determinant_2d( sub_seg1, sub_start ) / determinant_2d( sub_seg1, sub_seg2 );
    if ( seg2_t < 0 || 1 < seg2_t ) return false;

    return true;
}
cinder::vec2 get_hit_segment_intersection( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 )
{
    segment seg1( s1, e1 );
    segment seg2( s2, e2 );

    const vec2 sub_start = seg1.start - seg2.start;
    const vec2 sub_seg1 = seg1.end - seg1.start;
    const vec2 sub_seg2 = seg2.end - seg2.start;

    const double seg1_t = determinant_2d( sub_seg2, sub_start ) / determinant_2d( sub_seg1, sub_seg2 );
    if ( seg1_t < 0 || 1 < seg1_t ) return vec2( 0 );

    const double seg2_t = determinant_2d( sub_seg1, sub_start ) / determinant_2d( sub_seg1, sub_seg2 );
    if ( seg2_t < 0 || 1 < seg2_t ) return vec2( 0 );

    return vec2( sub_seg1.x * seg1_t + s1.x, sub_seg1.y * seg1_t + s1.y );
}
bool hit_point_rect( cinder::vec2 a, cinder::vec2 b, cinder::vec2 b_size )
{
    return b.x <= a.x && a.x <= b.x + b_size.x &&
        b.y <= a.y && a.y <= b.y + b_size.y;
}
bool hit_rect_rect( cinder::vec2 a, cinder::vec2 a_size, cinder::vec2 b, cinder::vec2 b_size )
{
    float mx1 = a.x;
    float my1 = a.y;
    float mx2 = a.x + a_size.x;
    float my2 = a.y + a_size.y;
    float ex1 = b.x;
    float ey1 = b.y;
    float ex2 = b.x + b_size.x;
    float ey2 = b.y + b_size.y;
    return mx1 <= ex2 && ex1 <= mx2 && my1 <= ey2 && ey1 <= my2;
}
bool hit_quad_quad( cinder::vec2 a1, cinder::vec2 b1, cinder::vec2 c1, cinder::vec2 d1, cinder::vec2 a2, cinder::vec2 b2, cinder::vec2 c2, cinder::vec2 d2 )
{
    return
        utility::hit_point_plane_2d( a2, a1, b1, c1, d1 ) || // ここから
        utility::hit_point_plane_2d( b2, a1, b1, c1, d1 ) || // a側からbの頂点を見て判断をしていきます。
        utility::hit_point_plane_2d( c2, a1, b1, c1, d1 ) ||
        utility::hit_point_plane_2d( d2, a1, b1, c1, d1 ) || // ここまで
        utility::hit_point_plane_2d( a1, a2, b2, c2, d2 ) || // ここから
        utility::hit_point_plane_2d( b1, a2, b2, c2, d2 ) || // b側からaの頂点を見て判断をしていきます。
        utility::hit_point_plane_2d( c1, a2, b2, c2, d2 ) ||
        utility::hit_point_plane_2d( d1, a2, b2, c2, d2 ) || // ここまで
        utility::hit_segment( d1, a1, c2, d2 ); // それでも当たらないものは 
                                                //      ┌------┐
                                                //  ┌---|------|---┐
                                                //  |   |      |   |
                                                //  |   |      |   |
                                                //  └---|------|---┘
                                                //      |      |
                                                //      └------┘
                                                // 上記のような形なので
                                                //      ↑
                                                //      |
                                                //      |
                                                //      |
                                                //  ←---|-----------
                                                //      |
                                                //      |
                                                // というように線分で当たり判定を取ります。
                                                // それでも当たらない場合は、windowの範囲外です。
}
bool hit_window_aabb( cinder::mat3 model_view_matrix, std::shared_ptr<node> const & object )
{
    auto _content_size = object->get_content_size( );
    auto ma = translate( model_view_matrix, vec2( 0.0F ) );
    auto a = vec2( ma[2][0], ma[2][1] );
    auto mb = translate( model_view_matrix, vec2( _content_size.x, 0.0F ) );
    auto b = vec2( mb[2][0], mb[2][1] );
    auto mc = translate( model_view_matrix, vec2( _content_size.x, _content_size.y ) );
    auto c = vec2( mc[2][0], mc[2][1] );
    auto md = translate( model_view_matrix, vec2( 0.0F, _content_size.y ) );
    auto d = vec2( md[2][0], md[2][1] );

    auto win_size = vec2( cinder::app::getWindowSize( ) );

    auto aabb = create_aabb( a, b, c, d );
    vec2
        pos = aabb.first,
        size = aabb.second;

    return hit_rect_rect( pos, size, vec2( 0.0F ), win_size );
}
float determinant_2d( cinder::vec2 a, cinder::vec2 b )
{
    return a.x * b.y - a.y * b.x;
}

std::pair<cinder::vec2, cinder::vec2> create_aabb( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d )
{
    std::vector<float> _x, _y;
    _x.emplace_back( a.x );
    _x.emplace_back( b.x );
    _x.emplace_back( c.x );
    _x.emplace_back( d.x );
    _y.emplace_back( a.y );
    _y.emplace_back( b.y );
    _y.emplace_back( c.y );
    _y.emplace_back( d.y );
    auto ret_x = std::minmax_element( std::begin( _x ), std::end( _x ) );
    auto ret_y = std::minmax_element( std::begin( _y ), std::end( _y ) );
    vec2 begin_pos, end_pos;
    begin_pos.x = *ret_x.first;
    end_pos.x = *ret_x.second;
    begin_pos.y = *ret_y.first;
    end_pos.y = *ret_y.second;

    vec2 pos = begin_pos;
    vec2 size = end_pos - begin_pos;

    return std::make_pair( pos, size );
}

}