#include "application/spk_application.hpp"

namespace spk
{
	void Application::_executeUpdateTick()
	{
		MONITOR_NAMED_ACTION_TIME_CONSUMPTION(L"Update time metric", _timeModule.updateTimeMetrics());
		MONITOR_NAMED_ACTION_TIME_CONSUMPTION(L"System module treat message", _systemModule.treatMessage());
		MONITOR_NAMED_ACTION_TIME_CONSUMPTION(L"GAPIM treat message", _GAPIM.treatMessage());
		MONITOR_NAMED_ACTION_TIME_CONSUMPTION(L"Mouse module treat message", _mouseModule.treatMessage());
		MONITOR_NAMED_ACTION_TIME_CONSUMPTION(L"Keyboard module treat message", _keyboardModule.treatMessage());
		MONITOR_NAMED_ACTION_TIME_CONSUMPTION(L"Widget update", _widgetModule.updateWidgets());
		MONITOR_NAMED_ACTION_TIME_CONSUMPTION(L"Mouse update", _mouseModule.updateMouse());
		MONITOR_NAMED_ACTION_TIME_CONSUMPTION(L"Keyboard update", _keyboardModule.updateKeyboard());

		//_timeModule.wait();
	}
}
