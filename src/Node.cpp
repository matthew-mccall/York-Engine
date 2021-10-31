#include <Node.hpp>

namespace york {
NodeBase::~NodeBase()
{
    for (auto node : m_children) {
        delete node;
    }
}
} // namespace york
