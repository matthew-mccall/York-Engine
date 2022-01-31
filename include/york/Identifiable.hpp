//
// Created by Matthew McCall on 12/19/21.
//

#ifndef YORK_IDENTIFIABLE_HPP
#define YORK_IDENTIFIABLE_HPP

#include <cstdint>

namespace york {

using id = std::uint64_t;

class Identifiable {
public:
    Identifiable();
    [[nodiscard]] id getID() const;
    ~Identifiable();


private:
     id m_id;
};

}

inline bool operator==(const york::Identifiable& lhs, const york::Identifiable& rhs) { return lhs.getID() == rhs.getID(); }

#endif // YORK_IDENTIFIABLE_HPP
