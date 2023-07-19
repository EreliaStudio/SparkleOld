#pragma once

#include "widget/spk_abstract_widget.hpp"
#include "input/spk_input.hpp"

namespace spk
{
	class InputManagerWidget : public spk::NoGraphicsWidget
	{
	private:
		bool _onUpdate();
	
	public:
		InputManagerWidget(const std::wstring& p_name);
		~InputManagerWidget();
	};
	
	void addInput(Input* p_input);
	void removeInput(Input* p_input);
}