#include "application/spk_application.hpp"
#include "widget/spk_widget_activity_scheduler_manager.hpp"
#include "widget/spk_widget_input_group_manager.hpp"
#include "debug/spk_profiler.hpp"

namespace spk
{
	void Application::setupJobs()
	{
		_renderContracts.push_back(addJob(L"Render IPS increment", [&]() { _profilerModule.updateFPS(); }));
		_renderContracts.push_back(addJob(L"API message pulling", [&]() { _APIModule.pullMessage(); }));
		_renderContracts.push_back(addJob(L"Window clearing", [&]() { _GAPIM.clear(); }));
		_renderContracts.push_back(addJob(L"Widget rendering", [&]() { _widgetModule.render(); }));
		_renderContracts.push_back(addJob(L"Window rendering", [&]() { _GAPIM.render(); }));
		_renderContracts.push_back(addJob(L"Waiting process", [&]() { _timeModule.wait(); }));

		_updateContracts.push_back(addJob(L"Updater", L"Time metrics update", [&]() { _timeModule.updateTimeMetrics(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Update IPS increment", [&]() { _profilerModule.updateUPS(); }));
		_updateContracts.push_back(addJob(L"Updater", L"System message treatement", [&]() { _systemModule.treatMessage(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Profiler update counter", [&]() { _profilerModule.updateCounters(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Window treat message", [&]() { _GAPIM.treatMessage(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Mouse treat message", [&]() { _mouseModule.treatMessage(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Keyboard treat message", [&]() { _keyboardModule.treatMessage(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Widget update", [&]() { _widgetModule.update(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Mouse update status", [&]() { _mouseModule.updateMouse(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Keyboard update status", [&]() { _keyboardModule.updateKeyboard(); }));
		_updateContracts.push_back(addJob(L"Updater", L"Waiting process", [&]() { _timeModule.wait(); }));
	}

	Application::Application(const std::wstring &p_title, const spk::Vector2Int &p_size) :
		spk::AbstractApplication(),
		_APIModule(),
		_timeModule(),
		_systemModule(_APIModule.systemQueue(), std::shared_ptr<AbstractApplication>(this, [](auto& tmp){})),
		_GAPIM(_APIModule.windowQueue(), p_title, p_size, &_APIModule),
		_mouseModule(_APIModule.mouseQueue()),
		_keyboardModule(_APIModule.keyboardQueue()),
		_widgetModule(),
		_profilerModule()
	{		
		resize(p_size);
	}

	Application::~Application()
	{
	}
	
	std::shared_ptr<spk::Widget::Canvas> Application::addCanvas(const std::filesystem::path& p_configurationFilePath)
	{
		auto result = std::make_shared<spk::Widget::Canvas>(p_configurationFilePath);

		result->setGeometry(spk::Vector2Int(0, 0), size());

		return (result);
	}

	void Application::resize(const spk::Vector2Int &p_size)
	{
		spk::Singleton<spk::Window>::instance()->resize(p_size);
	}
	
	const spk::Vector2UInt& Application::size() const
	{
		return (spk::Window::instance()->size());
	}

	void Application::setMaxFPS(const size_t& p_maxFPS)
	{
		_timeModule.setMaxFPS(p_maxFPS);
	}
}
