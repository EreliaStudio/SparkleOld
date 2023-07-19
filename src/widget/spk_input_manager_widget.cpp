#include "widget/spk_input_manager_widget.hpp"
#include "input/spk_input_manager.hpp"
#include "design_pattern/spk_singleton.hpp"

namespace spk
{
	bool InputManagerWidget::_onUpdate()
	{
		spk::Singleton<spk::InputManager>::instance()->update();

		return (false);
	}
	
	InputManagerWidget::InputManagerWidget(const std::wstring& p_name) : 
		spk::NoGraphicsWidget(p_name)
	{
		spk::Singleton<spk::InputManager>::instanciate();
	}
	
	InputManagerWidget::~InputManagerWidget()
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