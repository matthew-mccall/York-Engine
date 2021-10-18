#if !defined(YORK_APPLICATION_HPP)
#define YORK_APPLICATION_HPP

namespace york {

class Application {
public:
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onRender() = 0;
    bool getExit() const;
    virtual ~Application();

protected:
    bool m_exit = false;

    Application(/* args */);
};

Application* createApplication();

} // namespace york

#endif // YORK_APPLICATION_HPP
