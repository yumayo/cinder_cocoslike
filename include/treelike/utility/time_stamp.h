#pragma once
namespace treelike
{
namespace utility
{
// http://qiita.com/hurou927/items/a2d63837e731713c7a22
class time_stamp
{
    double _start_second = 0.0;
    double _end_second = 0.0;
public:
    double start( );
    double end( );
    double get_start_second( );
    double get_end_second( );
    double get_time_second( );
    double get_elapsed_second( );
};
}
}