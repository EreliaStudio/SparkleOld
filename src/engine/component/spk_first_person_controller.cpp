#include "engine/component/spk_first_person_controller_component.hpp"
#include "input/spk_key_input.hpp"
#include "input/spk_mouse_motion_input.hpp"
#include "input/spk_mouse_input.hpp"
#include "engine/spk_game_object.hpp"
#include "graphics/spk_window.hpp"

namespace spk
{
	bool FirstPersonController::_onUpdate()
	{
        for (size_t i = 0; i < _inputs.size(); i++)
        {
            _inputs[i]->update();
        }

        if (_movementRequested == true)
        {
            owner()->transform()->move(_requestedDeltaPosition.normalize() * _movementSpeed * spk::TimeMetrics::instance()->deltaTime() * 10);
            _requestedDeltaPosition = spk::Vector3(0, 0, 0);
            _movementRequested = false;
        }
        return (false);
	}
	
	void FirstPersonController::_onRender()
	{

	}

	void FirstPersonController::_startCameraRotation()
    {
        _lastMousePosition = spk::Mouse::instance()->position();
    }

    void FirstPersonController::_applyCameraRotation()
    {
        if (spk::Mouse::instance()->position() == _lastMousePosition)
            return ;

        spk::Vector2Int deltaMousePosition = spk::Mouse::instance()->position() - _lastMousePosition;

        spk::Vector3 rotation = spk::Vector3(-deltaMousePosition.y, -deltaMousePosition.x, 0) * spk::TimeMetrics::instance()->deltaTime() * _rotationSpeed;

        owner()->transform()->rotate(rotation);

        _lastMousePosition = spk::Mouse::instance()->position();
    }

                    // spk::Mouse::instance()->place(spk::Vector2Int(spk::Window::instance()->size() / spk::Vector2UInt(2, 2)));

	FirstPersonController::FirstPersonController() :
		Component(L"FirstPersonController"),
        _mouseControl(MouseControl::Free),
        _inputs{
                std::make_shared<spk::KeyInput>(spk::Keyboard::Z, spk::InputState::Down, 10, [&](){
                        _requestedDeltaPosition += (owner()->transform()->forward());
                        _movementRequested = true;
                    }),
                std::make_shared<spk::KeyInput>(spk::Keyboard::Q, spk::InputState::Down, 10, [&](){
                        _requestedDeltaPosition += (owner()->transform()->right());
                        _movementRequested = true;
                    }),
                std::make_shared<spk::KeyInput>(spk::Keyboard::S, spk::InputState::Down, 10, [&](){
                        _requestedDeltaPosition += (-owner()->transform()->forward());
                        _movementRequested = true;
                    }),
                std::make_shared<spk::KeyInput>(spk::Keyboard::D, spk::InputState::Down, 10, [&](){
                        _requestedDeltaPosition += (-owner()->transform()->right());
                        _movementRequested = true;
                    }),
                std::make_shared<spk::KeyInput>(spk::Keyboard::Space, spk::InputState::Down, 10, [&](){
                        _requestedDeltaPosition += (spk::Vector3(0, 1, 0));
                        _movementRequested = true;
                    }),
                std::make_shared<spk::KeyInput>(spk::Keyboard::LeftShift, spk::InputState::Down, 10, [&](){
                        _requestedDeltaPosition += (spk::Vector3(0, -1, 0));
                        _movementRequested = true;
                    }),
                std::make_shared<spk::KeyInput>(spk::Keyboard::A, spk::InputState::Down, 10, [&](){
                        if (_mouseControl != MouseControl::Captured)
                        {
                            owner()->transform()->rotate(spk::Vector3(0, 1, 0));
                        }
                    }),
                std::make_shared<spk::KeyInput>(spk::Keyboard::E, spk::InputState::Down, 10, [&](){
                        if (_mouseControl != MouseControl::Captured)
                        {
                            owner()->transform()->rotate(spk::Vector3(0, -1, 0));
                        }
                    }),
                std::make_shared<spk::MouseInput>(spk::Mouse::Button::Left, spk::InputState::Pressed, 0, [&](){
                        if (_mouseControl == MouseControl::PressedLeft)
                        {
                            _startCameraRotation();
                        }
                    }),
                std::make_shared<spk::MouseInput>(spk::Mouse::Button::Left, spk::InputState::Down, 1, [&](){
                        if (_mouseControl == MouseControl::PressedLeft)
                        {
                            _applyCameraRotation();
                        }
                    })
            }
	{

	}

    const float& FirstPersonController::movementSpeed() const
    {
        return (_movementSpeed);
    }
    
    void FirstPersonController::setMovementSpeed(const float& p_movementSpeed)
    {
        _movementSpeed = p_movementSpeed / 1000.0f;
    }
		
    const FirstPersonController::MouseControl& FirstPersonController::mouseControl() const
    {
        return (_mouseControl);
    }
    
    void FirstPersonController::setMouseControl(const FirstPersonController::MouseControl& p_mouseControl)
    {
        _mouseControl = p_mouseControl;
    }
}