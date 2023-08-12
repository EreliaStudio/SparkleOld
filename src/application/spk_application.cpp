#include "application/spk_application.hpp"
#include "widget/spk_widget_activity_scheduler_manager.hpp"
#include "widget/spk_widget_input_group_manager.hpp"

namespace spk
{
	void Application::setupJobs()
	{
		_renderContracts.push_back(addJob(L"Render IPS increment", [&]() { _profilerModule->increaseRenderIPS(); }));
		_renderContracts.push_back(addJob(L"API message pulling", [&]() { _APIModule->pullMessage(); }));
		_renderContracts.push_back(addJob(L"Window clearing", [&]() { _windowModule->clear(); }));
		_renderContracts.push_back(addJob(L"Widget rendering", [&]() { _widgetModule->render(); }));
		_renderContracts.push_back(addJob(L"Window rendering", [&]() { _windowModule->render(); }));

		_updateContracts.push_back(addJob(L"Updater", L"Update IPS increment", [&]() { _profilerModule->increaseUpdateIPS(); }));
		_updateContracts.push_back(addJob(L"Updater", L"System message treatement", [&]() { _systemModule->treatMessage(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Time metrics update", [&]() { _timeModule->updateTimeMetrics(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Profiler update data", [&]() { _profilerModule->updateData(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Window treat message", [&]() { _windowModule->treatMessage(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Mouse treat message", [&]() { _mouseModule->treatMessage(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Keyboard treat message", [&]() { _keyboardModule->treatMessage(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Widget update", [&]() { _widgetModule->update(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Mouse update status", [&]() { _mouseModule->updateMouse(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Mouse update status", [&]() { _keyboardModule->updateKeyboard(); }));
	}

	void Application::_initializeConfigurableValues(const Application::Configuration& p_configuration)
	{
		if (p_configuration.initActivityScheduler == true)
			addRootWidget<spk::Widget::ActivitySchedulerManager>(L"ActivitySchedulerManager")->activate();
		if (p_configuration.initInputGroupManager == true)
			addRootWidget<spk::Widget::InputGroupManager>(L"InputGroupManager")->activate();
	}

	Application::Application(const std::wstring &p_title, const spk::Vector2Int &p_size, const Configuration& p_configuration)
	{
		_APIModule = new spk::APIModule();

		_systemModule = new spk::SystemModule(_APIModule->systemQueue(), this);
		_timeModule = new spk::TimeModule();
		_profilerModule = new spk::ProfilerModule();

#ifdef _WIN32
		_windowModule = new spk::WindowModule(_APIModule->windowQueue(), p_title, p_size, _APIModule);
#elif __linux__
		_windowModule = new spk::WindowModule(_APIModule->windowQueue(), p_title, p_size);
#endif

		_mouseModule = new spk::MouseModule(_APIModule->mouseQueue());
		_keyboardModule = new spk::KeyboardModule(_APIModule->keyboardQueue());

		_widgetModule = new spk::WidgetModule();
		resize(p_size);

		_initializeConfigurableValues(p_configuration);
	}

	Application::~Application()
	{
		delete _APIModule;
		delete _systemModule;
		delete _timeModule;
		delete _windowModule;
		delete _mouseModule;
		delete _keyboardModule;
		delete _profilerModule;

		delete _widgetModule;
	}

	void Application::resize(const spk::Vector2Int &p_size)
	{
		spk::Singleton<spk::Window>::instance()->setGeometry(p_size);
	}
	
	const spk::Vector2Int& Application::size() const
	{
		return (spk::Window::instance()->size());
	}
}
