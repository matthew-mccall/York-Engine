/**
 * @file Application.hpp
 * @author Matthew McCall (m-mccall@outlook.com)
 * @brief Contains the declaration for the Application class.
 * @version 0.1
 * @date 2021-11-01
 *
 * @copyright Copyright (c) 2021, Matthew McCall and contributors. All Rights Reserved.
 *
 */

#if !defined(YORK_APPLICATION_HPP)
#define YORK_APPLICATION_HPP

#include "Event.hpp"

namespace york {

/**
 * @brief The base class for you Application. Inherit this and override Application::onUpdate() and Application::onRender() to implement your Application logic.
 *
 */
class Application : private EventHandler {
public:
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onRender() = 0;
    void onEvent(Event e) override;
    [[nodiscard]] bool getExit() const;
    virtual ~Application() = default;

protected:
    bool m_exit = false;

    Application() = default;
};

extern Application* createApplication();

} // namespace york

#endif // YORK_APPLICATION_HPP
