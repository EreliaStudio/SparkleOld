#pragma once

#include "engine/component/spk_component.hpp"
#include "engine/component/spk_transform_component.hpp"
#include "engine/spk_mesh.hpp"
#include "graphics/spk_pipeline.hpp"
#include "graphics/spk_texture.hpp"
#include "input/spk_input.hpp"

namespace spk
{
    /**
     * @brief Represents a first-person controller component in the Sparkle engine.
     *
     * This class is responsible for handling first-person camera movement and rotation,
     * providing functionalities for mouse and keyboard input processing.
     */
    class FirstPersonController : public Component
    {
    public:
        /**
         * @brief Enum for mouse control modes.
         */
        enum class MouseControl
        {
            Captured,    ///< Mouse captured and hidden, typically used for looking around.
            PressedLeft, ///< Left mouse button pressed.
            PressedRight,///< Right mouse button pressed.
            Free         ///< Mouse free and visible.
        };

    protected:
        spk::Vector2Int _lastMousePosition; ///< Last recorded mouse position.
        MouseControl _mouseControl; ///< Current mouse control mode.
        bool _movementRequested = false; ///< Flag to indicate if movement is requested.
        spk::Vector3 _requestedDeltaPosition; ///< Requested change in position.
        float _movementSpeed = 0.05f; ///< Movement speed in units per second.
        float _rotationSpeed = 0.05f; ///< Rotation speed.
        std::vector<std::shared_ptr<spk::Input>> _inputs; ///< List of input controls.

        virtual bool _onUpdate() override;
        virtual void _onRender() override;

        void _startCameraRotation(); ///< Begins camera rotation based on mouse input.
        void _applyCameraRotation(); ///< Applies camera rotation.

    public:
        /**
         * @brief Constructs a new First Person Controller object.
         *
         * Initializes the controller with default values for movement and rotation speeds.
         */
        FirstPersonController();

        /**
         * @brief Gets the movement speed.
         * 
         * @return const float& Movement speed in units per second.
         */
        const float& movementSpeed() const;

        /**
         * @brief Sets the movement speed.
         *
         * @param p_movementSpeed Movement speed in units per second.
         */
        void setMovementSpeed(const float& p_movementSpeed);

        /**
         * @brief Gets the current mouse control mode.
         * 
         * @return const MouseControl& Current mouse control mode.
         */
        const MouseControl& mouseControl() const;

        /**
         * @brief Sets the mouse control mode.
         *
         * @param p_mouseControl New mouse control mode.
         */
        void setMouseControl(const MouseControl& p_mouseControl);
    };
}
