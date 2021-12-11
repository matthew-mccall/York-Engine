#if !defined(YORK_GRAPHICS_HANDLE_HPP)
#define YORK_GRAPHICS_HANDLE_HPP

#include <vector>

namespace york::graphics {

class HandleBase {
public:
    void create();
    void destroy();
    [[nodiscard]] bool isCreated() const;
    virtual bool createImpl() = 0;
    virtual void destroyImpl() = 0;
    void addDependent(HandleBase& handle);
    void removeDependent(HandleBase& handle);
    virtual ~HandleBase() = default;

private:
    bool m_created = false;
    std::vector<HandleBase*> m_dependents;
};

template <typename T>
class Handle : public HandleBase {
public:
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
