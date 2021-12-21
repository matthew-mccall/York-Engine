#if !defined(YORK_GRAPHICS_HANDLE_HPP)
#define YORK_GRAPHICS_HANDLE_HPP

#include <vector>
#include <functional>

#include "york/Identifiable.hpp"

namespace york::graphics {

/**
 * A base for a system that allows handles to be attached to other handles and recreating handles and their dependents.
 */
class HandleBase : public Identifiable {
public:
    /**
     * @brief Creates a handle.
     *
     * If a handle is is created, it will be destroyed, including its dependents, then recreated, including its dependents.
     */
    void create();

    /**
     * Destroys a handle, including its dependents.
     */
    void destroy();

    /**
     * Gets whether is handle was created.
     *
     * @return Whether is handle was created.
     */
    [[nodiscard]] bool isCreated() const;

    /**
     * Adds a handle to its dependents list.
     *
     * @param handle The handle to add to its dependents list
     */
    void addDependent(HandleBase& handle);

    /**
     * Removes a handle from its dependents list.
     *
     * @param handle The handle to remove from its dependents list.
     */
    void removeDependent(HandleBase& handle);

    virtual ~HandleBase() = default;

protected:
    virtual bool createImpl() = 0;
    virtual void destroyImpl() = 0;

private:
    bool m_created = false;
    std::vector<std::reference_wrapper<HandleBase>> m_dependents;
};

/**
 * A template wrapper that allows any type to be treated as handle.
 *
 * @tparam T The handle type.
 */
template <typename T>
class Handle : public HandleBase {
public:

    /**
     * Gets the handle.
     *
     * @return The handle.
     */
    T getHandle()
    {
        return m_handle;
    }

    T* operator->()
    {
        return &m_handle;
    }

    T& operator*()
    {
        return m_handle;
    }

protected:
    T m_handle;
};

} // namespace york::graphics

#endif
