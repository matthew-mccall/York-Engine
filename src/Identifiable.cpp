//
// Created by Matthew McCall on 12/19/21.
//

#include <limits>
#include <random>
#include <unordered_set>

#include "york/Identifiable.hpp"

namespace {
std::unordered_set<uint64_t> s_usedIDs;
}

namespace york {

Identifiable::Identifiable()
{
    std::random_device randomDevice;
    std::default_random_engine randomEngine(randomDevice());
    std::uniform_int_distribution<uint64_t> uniformDist(std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max());

    std::uint64_t id = uniformDist(randomEngine);

    while (s_usedIDs.find(id) != s_usedIDs.end())
    {
        id = uniformDist(randomEngine);
    }

    s_usedIDs.insert(id);
    m_id = id;
}

uint64_t Identifiable::getID() const
{
    return m_id;
}

Identifiable::~Identifiable()
{
    s_usedIDs.erase(m_id);
}

}
