#pragma once
#include "base_player.h"
namespace audio
{
class buffer_player : public base_player
{
public:
    CREATE_H( buffer_player, std::string const& relative_path );
    bool init( std::string const& relative_path ) final override;
};
}