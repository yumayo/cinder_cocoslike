#pragma once
#include <cinder/Vector.h>
#include <memory>
#include <treelike/node.h>
#include <cinder/Camera.h>
#include <cinder/Frustum.h>
#include <cinder/AxisAlignedBox.h>
namespace treelike
{
namespace utility
{
// http://www.sousakuba.com/Programming/gs_hittest_point_triangle.html
bool hit_point_polygon_2d( cinder::vec2 point, cinder::vec2 a, cinder::vec2 b, cinder::vec2 c );

// http://marupeke296.com/COL_2D_No4_SquareToSquare.html
bool hit_point_plane_2d( cinder::vec2 point, cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d );
bool hit_point_plane_2d( cinder::vec2 point, hardptr<node> const& object );
bool hit_point_plane_2d_default_size( cinder::vec2 point, hardptr<node> const& object );

// https://gist.github.com/yoshiki/7702066
bool hit_segment( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 );
// Ç«Ç±Ç©ÇÁèEÇ¡ÇƒÇ´ÇΩÇÃÇ©ñYÇÍÇƒÇµÇ‹Ç¢Ç‹ÇµÇΩÅB
cinder::vec2 get_hit_segment_intersection( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 );

inline bool hit_point_rect( cinder::vec2 a, cinder::vec2 b, cinder::vec2 b_size )
{
    return b.x <= a.x && a.x <= b.x + b_size.x &&
        b.y <= a.y && a.y <= b.y + b_size.y;
}
// http://marupeke296.com/COL_2D_No4_SquareToSquare.html
bool hit_quad_quad( cinder::vec2 a1, cinder::vec2 b1, cinder::vec2 c1, cinder::vec2 d1,
                    cinder::vec2 a2, cinder::vec2 b2, cinder::vec2 c2, cinder::vec2 d2 );
inline float cross_z( cinder::vec2 a, cinder::vec2 b )
{
    return a.x * b.y - a.y * b.x;
}
// http://gyabo.sakura.ne.jp/tips/rect.html
inline bool hit_rect_rect( cinder::vec2 a, cinder::vec2 a_size, cinder::vec2 b, cinder::vec2 b_size )
{
    float const mx1 = a.x;
    float const my1 = a.y;
    float const mx2 = a.x + a_size.x;
    float const my2 = a.y + a_size.y;
    float const ex1 = b.x;
    float const ey1 = b.y;
    float const ex2 = b.x + b_size.x;
    float const ey2 = b.y + b_size.y;
    return mx1 <= ex2 && ex1 <= mx2 && my1 <= ey2 && ey1 <= my2;
}
inline std::pair<cinder::vec2, cinder::vec2> create_aabb( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d )
{
    float min_x = std::numeric_limits<float>::max( );
    float max_x = -std::numeric_limits<float>::max( );
    min_x = std::min( min_x, a.x );
    min_x = std::min( min_x, b.x );
    min_x = std::min( min_x, c.x );
    min_x = std::min( min_x, d.x );
    max_x = std::max( max_x, a.x );
    max_x = std::max( max_x, b.x );
    max_x = std::max( max_x, c.x );
    max_x = std::max( max_x, d.x );

    float min_y = std::numeric_limits<float>::max( );
    float max_y = -std::numeric_limits<float>::max( );
    min_y = std::min( min_y, a.y );
    min_y = std::min( min_y, b.y );
    min_y = std::min( min_y, c.y );
    min_y = std::min( min_y, d.y );
    max_y = std::max( max_y, a.y );
    max_y = std::max( max_y, b.y );
    max_y = std::max( max_y, c.y );
    max_y = std::max( max_y, d.y );

    return std::make_pair( cinder::vec2( min_x, min_y ), cinder::vec2( max_x, max_y ) );
}
inline std::pair<cinder::vec3, cinder::vec3> create_aabb( cinder::vec3 a, cinder::vec3 b, cinder::vec3 c, cinder::vec3 d,
                                                          cinder::vec3 e, cinder::vec3 f, cinder::vec3 g, cinder::vec3 h )
{
    float min_x = std::numeric_limits<float>::max( );
    float max_x = -std::numeric_limits<float>::max( );
    min_x = std::min( min_x, a.x );
    min_x = std::min( min_x, b.x );
    min_x = std::min( min_x, c.x );
    min_x = std::min( min_x, d.x );
    min_x = std::min( min_x, e.x );
    min_x = std::min( min_x, f.x );
    min_x = std::min( min_x, g.x );
    min_x = std::min( min_x, h.x );
    max_x = std::max( max_x, a.x );
    max_x = std::max( max_x, b.x );
    max_x = std::max( max_x, c.x );
    max_x = std::max( max_x, d.x );
    max_x = std::max( max_x, e.x );
    max_x = std::max( max_x, f.x );
    max_x = std::max( max_x, g.x );
    max_x = std::max( max_x, h.x );

    float min_y = std::numeric_limits<float>::max( );
    float max_y = -std::numeric_limits<float>::max( );
    min_y = std::min( min_y, a.y );
    min_y = std::min( min_y, b.y );
    min_y = std::min( min_y, c.y );
    min_y = std::min( min_y, d.y );
    min_y = std::min( min_y, e.y );
    min_y = std::min( min_y, f.y );
    min_y = std::min( min_y, g.y );
    min_y = std::min( min_y, h.y );
    max_y = std::max( max_y, a.y );
    max_y = std::max( max_y, b.y );
    max_y = std::max( max_y, c.y );
    max_y = std::max( max_y, d.y );
    max_y = std::max( max_y, e.y );
    max_y = std::max( max_y, f.y );
    max_y = std::max( max_y, g.y );
    max_y = std::max( max_y, h.y );

    float min_z = std::numeric_limits<float>::max( );
    float max_z = -std::numeric_limits<float>::max( );
    min_z = std::min( min_z, a.z );
    min_z = std::min( min_z, b.z );
    min_z = std::min( min_z, c.z );
    min_z = std::min( min_z, d.z );
    min_z = std::min( min_z, e.z );
    min_z = std::min( min_z, f.z );
    min_z = std::min( min_z, g.z );
    min_z = std::min( min_z, h.z );
    max_z = std::max( max_z, a.z );
    max_z = std::max( max_z, b.z );
    max_z = std::max( max_z, c.z );
    max_z = std::max( max_z, d.z );
    max_z = std::max( max_z, e.z );
    max_z = std::max( max_z, f.z );
    max_z = std::max( max_z, g.z );
    max_z = std::max( max_z, h.z );

    return std::make_pair( cinder::vec3( min_x, min_y, min_z ), cinder::vec3( max_x, max_y, max_z ) );
}
inline bool hit_window_aabb( cinder::mat3 model_view_matrix, softptr<node> object, std::pair<cinder::vec2, cinder::vec2>* aabb_dst = nullptr )
{
    auto const _content_size = object->get_content_size( );
    auto const ma = translate( model_view_matrix, cinder::vec2( 0.0F ) );
    auto const a = cinder::vec2( ma[2][0], ma[2][1] );
    auto const mb = translate( model_view_matrix, cinder::vec2( _content_size.x, 0.0F ) );
    auto const b = cinder::vec2( mb[2][0], mb[2][1] );
    auto const mc = translate( model_view_matrix, cinder::vec2( _content_size.x, _content_size.y ) );
    auto const c = cinder::vec2( mc[2][0], mc[2][1] );
    auto const md = translate( model_view_matrix, cinder::vec2( 0.0F, _content_size.y ) );
    auto const d = cinder::vec2( md[2][0], md[2][1] );
    auto const aabb = create_aabb( a, b, c, d );
    if ( aabb_dst ) *aabb_dst = create_aabb( a, b, c, d );
    return hit_rect_rect( aabb.first, aabb.second - aabb.first, cinder::vec2( 0 ), cinder::vec2( cinder::app::getWindowSize( ) ) );
}
inline bool hit_camera_aabb( cinder::mat4 model_view_matrix, cinder::Camera const& camera, softptr<node> object, std::pair<cinder::vec3, cinder::vec3>* aabb_dst = nullptr )
{
    if ( object->get_content_size_3d( ) == cinder::vec3( 0 ) ) return false;
    auto const _content_size = object->get_content_size_3d( );
    auto const ma = translate( model_view_matrix, cinder::vec3( 0.0F, 0.0F, 0.0F ) );
    auto const a = cinder::vec3( ma[3][0], ma[3][1], ma[3][2] );
    auto const mb = translate( model_view_matrix, cinder::vec3( _content_size.x, 0.0F, 0.0F ) );
    auto const b = cinder::vec3( mb[3][0], mb[3][1], mb[3][2] );
    auto const mc = translate( model_view_matrix, cinder::vec3( _content_size.x, _content_size.y, 0.0F ) );
    auto const c = cinder::vec3( mc[3][0], mc[3][1], mc[3][2] );
    auto const md = translate( model_view_matrix, cinder::vec3( 0.0F, _content_size.y, 0.0F ) );
    auto const d = cinder::vec3( md[3][0], md[3][1], md[3][2] );
    auto const me = translate( model_view_matrix, cinder::vec3( 0.0F, 0.0F, _content_size.z ) );
    auto const e = cinder::vec3( me[3][0], me[3][1], me[3][2] );
    auto const mf = translate( model_view_matrix, cinder::vec3( _content_size.x, 0.0F, _content_size.z ) );
    auto const f = cinder::vec3( mf[3][0], mf[3][1], mf[3][2] );
    auto const mg = translate( model_view_matrix, cinder::vec3( _content_size.x, _content_size.y, _content_size.z ) );
    auto const g = cinder::vec3( mg[3][0], mg[3][1], mg[3][2] );
    auto const mh = translate( model_view_matrix, cinder::vec3( 0.0F, _content_size.y, _content_size.z ) );
    auto const h = cinder::vec3( mh[3][0], mh[3][1], mh[3][2] );
    auto const aabb = create_aabb( a, b, c, d, e, f, g, h );
    if ( aabb_dst ) *aabb_dst = aabb;
    cinder::Frustum frustum( camera.getProjectionMatrix( ) * camera.getViewMatrix( ) );
    return frustum.intersects( cinder::AxisAlignedBox( aabb.first, aabb.second ) );
}
}

}