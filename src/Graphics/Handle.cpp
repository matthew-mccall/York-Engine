#include "york/Graphics/Handle.hpp"

namespace york::graphics {

void HandleBase::create()
{
    this->destroy();

    for (HandleBase& dependency : m_dependencies) {
        if (!dependency.isCreated()) {
            dependency.create();
        }
    }

    this->createImpl();
    m_created = true;

    for (HandleBase& dependent : m_dependents) {
        if (!dependent.isCreated()) {
            dependent.create();
        }
    }
}

void HandleBase::destroy()
{
    for (HandleBase& dependent : m_dependents) {
        if (dependent.isCreated()) {
            dependent.destroy();
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
    m_dependents.emplace_back(handle);
}

void HandleBase::removeDependent(HandleBase& handle)
{
    for (auto i = m_dependents.begin(); i != m_dependents.end(); i++) {
        if (handle == *i) {
            m_dependents.erase(i);
            break;
        }
    }
}

void HandleBase::addDependency(HandleBase& handle)
{
    handle.addDependent(*this);
    m_dependencies.emplace_back(handle);
}

void HandleBase::removeDependency(HandleBase& handle)
{
    handle.removeDependent(*this);
    for (auto i = m_dependencies.begin(); i != m_dependencies.end(); i++) {
        if (handle == *i) {
            m_dependencies.erase(i);
            break;
        }
    }}

HandleBase::~HandleBase()
{
    for (HandleBase& dependency : m_dependencies) {
        dependency.removeDependent(*this);
    }
}

} // namespace york::graphics
