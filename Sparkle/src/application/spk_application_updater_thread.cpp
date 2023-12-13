#include "application/spk_application.hpp"

namespace spk
{
	void Application::_executeUpdateTick()
	{
		static auto& timeModuleMetric = _profilerModule.profiler().metrics<TimeConsumption>(L"Update time metric");
		static auto& systemModuleMetric = _profilerModule.profiler().metrics<TimeConsumption>(L"System module treat message");
		static auto& GAPIMMetric = _profilerModule.profiler().metrics<TimeConsumption>(L"GAPIM treat message");
		static auto& mouseModuleMetric = _profilerModule.profiler().metrics<TimeConsumption>(L"Mouse module treat message");
		static auto& keyboardModuleMetric = _profilerModule.profiler().metrics<TimeConsumption>(L"Keyboard module treat message");
		static auto& widgetModuleMetric = _profilerModule.profiler().metrics<TimeConsumption>(L"Widget update");
		static auto& mouseUpdateMetric = _profilerModule.profiler().metrics<TimeConsumption>(L"Mouse update");
		static auto& keyboardUpdateMetric = _profilerModule.profiler().metrics<TimeConsumption>(L"Keyboard update");

		{
			spk::ScopedTimeConsumptionMetrics metric(timeModuleMetric);
			_timeModule.updateTimeMetrics();
		}
		{
			spk::ScopedTimeConsumptionMetrics metric(systemModuleMetric);
			_systemModule.treatMessage();
		}
		{
			spk::ScopedTimeConsumptionMetrics metric(GAPIMMetric);
			_GAPIM.treatMessage();
		}
		{
			spk::ScopedTimeConsumptionMetrics metric(mouseModuleMetric);
			_mouseModule.treatMessage();
		}
		{
			spk::ScopedTimeConsumptionMetrics metric(keyboardModuleMetric);
			_keyboardModule.treatMessage();
		}
		{
			spk::ScopedTimeConsumptionMetrics metric(widgetModuleMetric);
			_widgetModule.update();
		}
		{
			spk::ScopedTimeConsumptionMetrics metric(mouseUpdateMetric);
			_mouseModule.updateMouse();
		}
		{
			spk::ScopedTimeConsumptionMetrics metric(keyboardUpdateMetric);
			_keyboardModule.updateKeyboard();
		}
		//_timeModule.wait();
	}
}
