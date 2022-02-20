//
// Created by Matthew McCall on 12/19/21.
//

#ifndef YORK_LAYER_HPP
#define YORK_LAYER_HPP

#include "Event.hpp"
#include "Identifiable.hpp"

namespace york {

class Layer : public EventHandler {
public:
    Layer(Registry& registry);

    /**
     * A function that will be called when the layer is attached to the stack. Use this for initialization.
     */
    virtual void onAttach() { }

    /**
     * A function that will be called on every tick. Override this to implement your game logic.
     *
     * @param deltaTime The time since last tick.
     */
    virtual void onUpdate(float deltaTime) { }

    /**
     * A function that will be called on every frame. Override this to implement your rendering logic.
     */
    virtual void onRender() { }

    /**
     * A function that will be called when the layer is detached from the stack. Use this to clean up.
     */
    virtual void onDetach() { }

    [[nodiscard]] bool getExit() const;

    virtual ~Layer() = default;

protected:
    void requestExit();

private:
    bool m_exit = false;
};

}

#endif // YORK_LAYER_HPP
