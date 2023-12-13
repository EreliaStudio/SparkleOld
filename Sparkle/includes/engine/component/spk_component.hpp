#pragma once

#include <memory>
#include "design_pattern/spk_activable_object.hpp"
#include "profiler/spk_time_consumption_metrics.hpp"

namespace spk
{
    class GameObject;

    /**
     * @brief Represents a component that can be attached to a GameObject.
     *
     * Components are reusable pieces of functionality that can be attached to game objects.
     * They define specific behaviors or properties, such as rendering or physics.
     */
    class Component
    {
    protected:
        std::shared_ptr<GameObject> _owner; ///< The GameObject this component is attached to.
        std::wstring _name; ///< Name of the component.
		spk::TimeConsumption& _timeConsumptionMetrics;

        /**
         * @brief Virtual method for updating the component.
         *
         * This method should be overridden to define how the component updates each frame.
         * 
         * @return bool True if the update is successful, false otherwise.
         */
        virtual bool _onUpdate() = 0;

        /**
         * @brief Virtual method for rendering the component.
         *
         * This method should be overridden to define how the component is rendered.
         */
        virtual void _onRender() = 0;

    public:
        /**
         * @brief Construct a new Component object.
         *
         * @param p_name The name of the component.
         */
        Component(const std::wstring& p_name);

        /**
         * @brief Destroy the Component object.
         */
        ~Component();

        /**
         * @brief Gets the owner GameObject of this component.
         *
         * @return std::shared_ptr<GameObject> A shared pointer to the owning GameObject.
         */
        std::shared_ptr<GameObject> owner();

        /**
         * @brief Gets the owner GameObject of this component (const version).
         *
         * @return std::shared_ptr<const GameObject> A const shared pointer to the owning GameObject.
         */
        std::shared_ptr<const GameObject> owner() const;

        /**
         * @brief Gets the name of the component.
         *
         * @return const std::wstring& Reference to the component's name.
         */
        const std::wstring& name() const;

        /**
         * @brief Gets the full name of the component, including its owner's name.
         *
         * @return std::wstring The full name of the component.
         */
        std::wstring fullname() const;

        /**
         * @brief Updates the component.
         *
         * This method calls the _onUpdate virtual method and should be used in the game loop.
         * 
         * @return bool True if the update is successful, false otherwise.
         */
        bool update();

        /**
         * @brief Renders the component.
         *
         * This method calls the _onRender virtual method and should be used in the rendering loop.
         */
        void render();
    };
}
