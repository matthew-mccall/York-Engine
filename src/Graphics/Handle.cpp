#include "york/Graphics/Handle.hpp"

namespace york::graphics {

void HandleBase::create()
{
    if (m_created) {
        destroyImpl();
    }

    this->createImpl();
    m_created = true;
}

void HandleBase::destroy()
{
    if (m_created) {
        this->destroyImpl();
    }

    m_created = false;
}

} // namespace york::graphics
