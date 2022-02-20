//
// Created by Matthew McCall on 2/20/22.
//

#include "york/Registry.hpp"

namespace york {

Registry& Registry::getInstance()
{
    static Registry s_instance;
    return s_instance;
}

}