#include "widget/spk_input_manager_widget.hpp"
#include "input/spk_input_manager.hpp"
#include "design_pattern/spk_singleton.hpp"

namespace spk::Widget
{
	bool InputManager::_onUpdate()
	{
		spk::Singleton<spk::InputManager>::instance()->update();

		return (false);
	}
	
	InputManager::InputManager(const std::wstring& p_name) : 
		spk::Widget::NoGraphics(p_name)
	{
		spk::Singleton<spk::InputManager>::instanciate();
	}
	
	InputManager::~InputManager()
	{
		spk::Singleton<spk::InputManager>::release();
	}

	void addInput(Input* p_input)
	{
		spk::Singleton<spk::InputManager>::instance()->addInput(p_input);
	}

	void removeInput(Input* p_input)
	{
		spk::Singleton<spk::InputManager>::instance()->removeInput(p_input);
	}
}