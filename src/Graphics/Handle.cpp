#include "york/Graphics/Handle.hpp"

namespace york::graphics {

void HandleBase::create()
{
    this->destroy();
    this->createImpl();

    for (HandleBase* dependent : m_dependents) {
        dependent->create();
    }

    m_created = true;
}

void HandleBase::destroy()
{
    for (HandleBase* dependent : m_dependents) {
        if (dependent->isCreated()) {
            dependent->destroy();
        }
    }

    if (m_created) {
        this->destroyImpl();
    }

    m_created = false;
}
bool HandleBase::isCreated() const
{
    return m_created;
}

void HandleBase::addDependent(HandleBase& handle)
{
    m_dependents.push_back(&handle);
}

void HandleBase::removeDependent(HandleBase& handle)
{
    for (auto i = m_dependents.begin(); i != m_dependents.end(); i++) {
        if (&handle == *i) {
            m_dependents.erase(i);
            break;
        }
    }
}

} // namespace york::graphics
