#include "york/Log.hpp"
#include "york/Graphics/Handle.hpp"

namespace york::graphics {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void HandleBase::create()
{
    this->destroy(); // m_created will be set false here

    for (HandleBase& dependency : m_dependencies) {
        if (!dependency.isCreated()) {
            dependency.create(); // however, this particular handle might be created as part of dependency resolution lower down in the stack
        }
    }

    if (!m_created) { // If this is the case, we want to prevent double creation once we move back up the stack
        m_created = this->createImpl();
    }

    for (auto i = m_dependents.begin(); i != m_dependents.end(); i++) { // Dependents may change as other dependents are created.
        if (!i->get().isCreated()) {
            i->get().create();
            i = m_dependents.begin();
        }
    }
}

void HandleBase::destroy()
{
    for (auto i = m_dependents.begin(); i != m_dependents.end(); i++) { // Dependents may change as other dependents are created.
        if (i->get().isCreated()) {
            i->get().destroy();
            i = m_dependents.begin();
        }
    }

    if (m_created) {
        this->destroyImpl();
    }

    m_created = false;
}

#pragma clang diagnostic pop

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
    }
}

HandleBase::~HandleBase()
{
    for (HandleBase& dependency : m_dependencies) {
        dependency.removeDependent(*this);
    }
}

} // namespace york::graphics
