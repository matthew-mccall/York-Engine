//
// Created by Matthew McCall on 12/3/21.
//

#ifndef YORK_GRAPHICS_REQUESTABLEITEM_HPP
#define YORK_GRAPHICS_REQUESTABLEITEM_HPP

#include <string>

namespace york {

struct RequestableItem {
    std::string name;
    bool required = true;
};

}

#endif // YORK_GRAPHICS_REQUESTABLEITEM_HPP
