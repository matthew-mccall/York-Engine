//
// Created by Matthew McCall on 12/22/21.
//

#include "york/Exit.hpp"

namespace {
bool s_exit = false;
}

namespace york {

void requestExit()
{
    s_exit = true;
}

bool getExit()
{
    return s_exit;
}

}