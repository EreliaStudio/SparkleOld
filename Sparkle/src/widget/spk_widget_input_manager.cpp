#include "widget/spk_widget_input_group_manager.hpp"
#include "input/spk_input_group.hpp"
#include "design_pattern/spk_singleton.hpp"

namespace spk::Widget
{
	bool InputGroupManager::_onUpdate()
	{
		spk::Singleton<spk::InputGroup>::instance()->update();

		return (false);
	}
	
	InputGroupManager::InputGroupManager(const std::wstring& p_name) : 
		spk::Widget::NoGraphics(p_name)
	{
		spk::Singleton<spk::InputGroup>::instanciate();
	}
	
	InputGroupManager::~InputGroupManager()
	{
		spk::Singleton<spk::InputGroup>::release();
	}

	void addInput(Input* p_input)
	{
		spk::Singleton<spk::InputGroup>::instance()->addInput(p_input);
	}

	void removeInput(Input* p_input)
	{
		spk::Singleton<spk::InputGroup>::instance()->removeInput(p_input);
	}
}