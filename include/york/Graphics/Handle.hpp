#if !defined(YORK_GRAPHICS_HANDLE_HPP)
#define YORK_GRAPHICS_HANDLE_HPP

namespace york::graphics {

class HandleBase {
public:
    void create();
    void destroy();
    virtual bool createImpl() = 0;
    virtual void destroyImpl() = 0;
    virtual ~HandleBase() = default;

private:
    bool m_created = false;
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

protected:
    T m_handle;
};

} // namespace york::graphics

#endif
