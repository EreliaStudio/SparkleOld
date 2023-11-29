#include "application/spk_application.hpp"
#include "widget/spk_widget_input_group_manager.hpp"
#include "spk_basic_functions.hpp"
#include "debug/spk_profiler.hpp"
#include "graphics/spk_window.hpp"
#include "threading/spk_thread.hpp"
#include "engine/component/spk_mesh_renderer_component.hpp"

namespace spk
{
	Application::Application(const std::wstring& p_title, const spk::Vector2Int& p_size) :
		_errorCode(0),
		_isRunning(false),
		_APIModule(),
		_timeModule(),
		_systemModule(_APIModule.systemQueue(), this),
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
		_isRunning = false;
	}

	void Application::_renameThread(const std::wstring& p_threadName)
	{
		spk::cout.setPrefix(p_threadName);
		spk::Profiler::instance()->defineThreadName(p_threadName);
	}
		
	bool Application::isRunning() const
	{
		return (_isRunning);
	}

	void Application::_initializeShaders()
	{
		MeshRenderer::initializeMeshRendererShader();
	}

	int Application::run()
	{
		_errorCode = 0;
		_isRunning = true;

		spk::Thread updaterThread(spk::Thread::LaunchMethod::Delayed, L"Updater", [&](){
			while (_isRunning == true)
			{
				_timeModule.updateTimeMetrics();
				_profilerModule.updateUPS();
				_profilerModule.updateCounters();
				_systemModule.treatMessage();
				_GAPIM.treatMessage();
				_mouseModule.treatMessage();
				_keyboardModule.treatMessage();
				_widgetModule.update();
				_mouseModule.updateMouse();
				_keyboardModule.updateKeyboard();
			}
		});

		_renameThread(L"Renderer");
		spk::Profiler::instance()->fpsCounter();
		updaterThread.start();
		_initializeShaders();
		while (_isRunning)
		{
			if (updaterThread.isActive() == false)
			{
				quit(1);
			}

			_profilerModule.updateFPS();
			_APIModule.pullMessage();
			_GAPIM.clear();
			_widgetModule.render();
			_GAPIM.render();
			_timeModule.wait();
		}

		updaterThread.join();

		spk::cout.setPrefix(L"");

		return _errorCode;
	}

	void Application::quit(int p_errorCode)
	{
		_errorCode = p_errorCode;
		_isRunning = false;
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