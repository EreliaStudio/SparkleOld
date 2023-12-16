#include "application/spk_application.hpp"

namespace spk
{
	void Application::_executeRenderTick()
	{
		_APIModule.pullMessage();
		_GAPIM.clear();
		_widgetModule.render();
		_GAPIM.render();
		_timeModule.wait();
	}
}
