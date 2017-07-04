#include <treelike/utility/collision.h>
#include <cinder/gl/gl.h>
#include <limits>
using namespace cinder;
namespace treelike
{
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
    vec2 const AB = b - a;
    vec2 const BP = point - b;
    vec2 const BC = c - b;
    vec2 const CP = point - c;
    vec2 const CA = a - c;
    vec2 const AP = point - a;
    double const c1 = cross_z( AB, BP );
    double const c2 = cross_z( BC, CP );
    double const c3 = cross_z( CA, AP );
    return c1 >= 0.0 && c2 >= 0.0 && c3 >= 0.0;
}
bool hit_point_plane_2d( cinder::vec2 point, cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d )
{
    vec2 const AB = b - a;
    vec2 const BP = point - b;
    vec2 const BC = c - b;
    vec2 const CP = point - c;
    vec2 const CD = d - c;
    vec2 const DP = point - d;
    vec2 const DA = a - d;
    vec2 const AP = point - a;
    float const c1 = cross_z( AB, BP );
    float const c2 = cross_z( BC, CP );
    float const c3 = cross_z( CD, DP );
    float const c4 = cross_z( DA, AP );
    return c1 >= 0.0 && c2 >= 0.0 && c3 >= 0.0 && c4 >= 0.0;
}
bool hit_point_plane_2d( cinder::vec2 point, hardptr<node> const & object )
{
    auto const mat = object->get_world_matrix( );
    auto const _content_size = object->get_content_size( );
    auto const _anchor_point = object->get_anchor_point( );
    auto const _position = object->get_position( );
    auto const _scale = object->get_scale( );
    auto const _rotation = object->get_rotation( );

    auto obj = mat;
    obj = translate( obj, _position );
    obj = scale( obj, _scale );
    obj = rotate( obj, _rotation );
    obj = translate( obj, -_content_size * _anchor_point );

    auto const ma = translate( obj, vec2( 0.0F, 0.0F ) );
    auto const a = vec2( ma[2][0], ma[2][1] );
    auto const mb = translate( obj, vec2( _content_size.x, 0.0F ) );
    auto const b = vec2( mb[2][0], mb[2][1] );
    auto const mc = translate( obj, vec2( _content_size.x, _content_size.y ) );
    auto const c = vec2( mc[2][0], mc[2][1] );
    auto const md = translate( obj, vec2( 0.0F, _content_size.y ) );
    auto const d = vec2( md[2][0], md[2][1] );

    return utility::hit_point_plane_2d( point, a, b, c, d );
}
bool hit_point_plane_2d_default_size( cinder::vec2 point, hardptr<node> const & object )
{
    auto const mat = object->get_world_matrix( );
    auto const _content_size = object->get_content_size( );
    auto const _anchor_point = object->get_anchor_point( );
    auto const _position = object->get_position( );
    auto const _scale = object->get_scale( );
    auto const _rotation = object->get_rotation( );

    auto obj = mat;
    obj = translate( obj, _position );
    //obj = scale( obj, _scale ); ボタンの拡大アクション時にスケールごと当たり判定を起こしたくないので。
    obj = rotate( obj, _rotation );
    obj = translate( obj, -_content_size * _anchor_point );

    auto const ma = translate( obj, vec2( 0.0F, 0.0F ) );
    auto const a = vec2( ma[2][0], ma[2][1] );
    auto const mb = translate( obj, vec2( _content_size.x, 0.0F ) );
    auto const b = vec2( mb[2][0], mb[2][1] );
    auto const mc = translate( obj, vec2( _content_size.x, _content_size.y ) );
    auto const c = vec2( mc[2][0], mc[2][1] );
    auto const md = translate( obj, vec2( 0.0F, _content_size.y ) );
    auto const d = vec2( md[2][0], md[2][1] );

    return utility::hit_point_plane_2d( point, a, b, c, d );
}
bool hit_segment( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 )
{
    segment const seg1( s1, e1 );
    segment const seg2( s2, e2 );

    vec2 const sub_start = seg1.start - seg2.start;
    vec2 const sub_seg1 = seg1.end - seg1.start;
    vec2 const sub_seg2 = seg2.end - seg2.start;

    float const seg1_t = cross_z( sub_seg2, sub_start ) / cross_z( sub_seg1, sub_seg2 );
    if ( seg1_t < 0 || 1 < seg1_t ) return false;

    float const seg2_t = cross_z( sub_seg1, sub_start ) / cross_z( sub_seg1, sub_seg2 );
    if ( seg2_t < 0 || 1 < seg2_t ) return false;

    return true;
}
cinder::vec2 get_hit_segment_intersection( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 )
{
    segment const seg1( s1, e1 );
    segment const seg2( s2, e2 );

    vec2 const sub_start = seg1.start - seg2.start;
    vec2 const sub_seg1 = seg1.end - seg1.start;
    vec2 const sub_seg2 = seg2.end - seg2.start;

    float const seg1_t = cross_z( sub_seg2, sub_start ) / cross_z( sub_seg1, sub_seg2 );
    if ( seg1_t < 0 || 1 < seg1_t ) return vec2( 0 );

    float const seg2_t = cross_z( sub_seg1, sub_start ) / cross_z( sub_seg1, sub_seg2 );
    if ( seg2_t < 0 || 1 < seg2_t ) return vec2( 0 );

    return vec2( sub_seg1.x * seg1_t + s1.x, sub_seg1.y * seg1_t + s1.y );
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
}
}