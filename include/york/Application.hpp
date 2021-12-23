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

#include "LayerStack.hpp"

namespace york {

/**
 * @brief The base class for your Application. Inherit this and override Application::onUpdate() and Application::onRender() to implement your Application logic.
 *
 */
class Application {
public:
    void pushLayer(Layer& layer);
    void popLayer(Layer& layer);

    LayerStack& getLayerStack();

private:
    LayerStack m_defaultLayerStack;

};

/**
 * Implement this function to create your application. It will be used by the engine.
 *
 * @return
 */
extern Application* createApplication();

} // namespace york

#endif // YORK_APPLICATION_HPP
