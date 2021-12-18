//
// Created by Matthew McCall on 12/3/21.
//

#ifndef YORK_GRAPHICS_REQUESTABLEITEM_HPP
#define YORK_GRAPHICS_REQUESTABLEITEM_HPP

#include <string>

namespace york {

/**
 * A convenience structure for representing required or optional items that can be requested.
 */
struct RequestableItem {
    /**
     * The name of the item.
     */
    std::string name;
    /**
     * Whether it is required.
     */
    bool required = true;
};

}

#endif // YORK_GRAPHICS_REQUESTABLEITEM_HPP
