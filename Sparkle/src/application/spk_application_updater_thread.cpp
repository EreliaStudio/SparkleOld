#include "application/spk_application.hpp"

namespace spk
{
	void Application::_executeUpdateTick()
	{
		_timeModule.updateTimeMetrics();
		_systemModule.treatMessage();
		_GAPIM.treatMessage();
		_mouseModule.treatMessage();
		_keyboardModule.treatMessage();
		_widgetModule.update();
		_mouseModule.updateMouse();
		_keyboardModule.updateKeyboard();
	}
	
	void Application::_updateProcess()
	{
		while (_isRunning == true)
		{
			_executeUpdateTick();
		}
	}
}
