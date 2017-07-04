#pragma once
#include <cinder/Vector.h>
#include <memory>
#include <treelike/node.h>
namespace treelike
{
namespace utility
{
// http://www.sousakuba.com/Programming/gs_hittest_point_triangle.html
bool hit_point_polygon_2d( cinder::vec2 point, cinder::vec2 a, cinder::vec2 b, cinder::vec2 c );

// http://marupeke296.com/COL_2D_No4_SquareToSquare.html
bool hit_point_plane_2d( cinder::vec2 point, cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d );
bool hit_point_plane_2d( cinder::vec2 point, std::shared_ptr<node> const& object );
bool hit_point_plane_2d_default_size( cinder::vec2 point, std::shared_ptr<node> const& object );

// https://gist.github.com/yoshiki/7702066
bool hit_segment( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 );
// ‚Ç‚±‚©‚çE‚Á‚Ä‚«‚½‚Ì‚©–Y‚ê‚Ä‚µ‚Ü‚¢‚Ü‚µ‚½B
cinder::vec2 get_hit_segment_intersection( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 );

bool hit_point_rect( cinder::vec2 point, cinder::vec2 b, cinder::vec2 b_size );

// http://gyabo.sakura.ne.jp/tips/rect.html
bool hit_rect_rect( cinder::vec2 a, cinder::vec2 a_size,
                    cinder::vec2 b, cinder::vec2 b_size );

// http://marupeke296.com/COL_2D_No4_SquareToSquare.html
bool hit_quad_quad( cinder::vec2 a1, cinder::vec2 b1, cinder::vec2 c1, cinder::vec2 d1,
                    cinder::vec2 a2, cinder::vec2 b2, cinder::vec2 c2, cinder::vec2 d2 );
bool hit_window_aabb( cinder::mat3 model_view_matrix, std::shared_ptr<node> const& object );

float determinant_2d( cinder::vec2 a, cinder::vec2 b );

std::pair<cinder::vec2, cinder::vec2> create_aabb( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d );
}

}