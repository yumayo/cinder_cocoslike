#pragma once
#include "node.h"
namespace treelike
{
class scene : public node
{
public:
    virtual ~scene( ) { }
    CREATE_H( scene );
    bool init( );
};
}