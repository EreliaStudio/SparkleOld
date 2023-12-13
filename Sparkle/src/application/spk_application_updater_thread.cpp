#include "application/spk_application.hpp"

namespace spk
{
	void Application::_executeUpdateTick()
	{
		MONITOR_DURATION(L"Update time metric", _timeModule.updateTimeMetrics());
		MONITOR_DURATION(L"System module treat message", _systemModule.treatMessage());
		MONITOR_DURATION(L"GAPIM treat message", _GAPIM.treatMessage());
		MONITOR_DURATION(L"Mouse module treat message", _mouseModule.treatMessage());
		MONITOR_DURATION(L"Keyboard module treat message", _keyboardModule.treatMessage());
		MONITOR_DURATION(L"Widget update", _widgetModule.update());
		MONITOR_DURATION(L"Mouse update", _mouseModule.updateMouse());
		MONITOR_DURATION(L"Keyboard update", _keyboardModule.updateKeyboard());

		_timeModule.wait();
	}
}
