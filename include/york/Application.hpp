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
 * @brief The base class for your Application. Inherit this and override Application::onUpdate() and Application::onRender() to implement your Application logic.
 *
 */
class Application : private EventHandler {
public:
    /**
     * A function that will be called on every tick. Override this to implement your game logic.
     *
     * @param deltaTime The time since last tick.
     */
    virtual void onUpdate(float deltaTime) = 0;

    /**
     * A function that will be called on every frame. Override this to implement your rendering logic.
     */
    virtual void onRender() = 0;

    /**
     * A function to handle events. Override this to intercept events.
     * @param e
     */
    void onEvent(Event e) override;

    /**
     * Used by the engine to determine whether you want to shutdown.
     * @return
     */
    [[nodiscard]] bool getExit() const;
    virtual ~Application() = default;

protected:
    /**
     * Set this to true when you want to exit.
     */
    bool m_exit = false;

    Application() = default;
};

/**
 * Implement this function to create your application. It will be used by the engine.
 *
 * @return
 */
extern Application* createApplication();

} // namespace york

#endif // YORK_APPLICATION_HPP
