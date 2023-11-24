#pragma once

#include "engine/component/spk_component.hpp"
#include "engine/component/spk_transform_component.hpp"
#include "engine/spk_mesh.hpp"
#include "graphics/spk_pipeline.hpp"
#include "graphics/spk_texture.hpp"
#include "input/spk_input.hpp"

namespace spk
{
	class FirstPersonController : public Component
	{
	public:
		enum class MouseControl
		{
			Captured,
			PressedLeft,
			PressedRight,
			Free
		};

	protected:
    	spk::Vector2Int _lastMousePosition;
		MouseControl _mouseControl;
		bool _movementRequested = false;
    	spk::Vector3 _requestedDeltaPosition;
		float _movementSpeed = 0.05f; //In Unit per second
		float _rotationSpeed = 0.25f;
    	std::vector<std::shared_ptr<spk::Input>> _inputs;

		virtual bool _onUpdate() override;
		virtual void _onRender() override;

		void _startCameraRotation();
		void _applyCameraRotation();

	public:
		FirstPersonController();

		const float& movementSpeed() const;
		/**
		 * @param p_movementSpeed Movement speed expressed as Unit per second
		*/
		void setMovementSpeed(const float& p_movementSpeed);

		const MouseControl& mouseControl() const;
		void setMouseControl(const MouseControl& p_mouseControl);
	};
}