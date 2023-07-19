#pragma once

#include "widget/spk_widget_interface.hpp"
#include "input/spk_input.hpp"

namespace spk::Widget
{
	class InputManager : public spk::Widget::NoGraphics
	{
	private:
		bool _onUpdate();
	
	public:
		InputManager(const std::wstring& p_name);
		~InputManager();
	};
	
	void addInput(Input* p_input);
	void removeInput(Input* p_input);
}