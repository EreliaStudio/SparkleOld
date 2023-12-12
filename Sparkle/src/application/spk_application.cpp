#include "application/spk_application.hpp"

#include "threading/spk_thread.hpp"

#include "system/spk_timer.hpp"

#include "engine/component/spk_mesh_renderer_component.hpp"

namespace spk
{
	void Application::_initialisationProcess()
	{
		_errorID = 0;
		_isRunning = true;

		MeshRenderer::initializeMeshRendererShader();
	}
	
	void Application::_updateProcess()
	{
		while (_isRunning == true)
		{
			_upsCounter += 10;

			_executeUpdateTick();
		}
	}

	void Application::_renderProcess()
	{
		spk::Timer counterTimer(100);

		counterTimer.start();

		while (_isRunning)
		{
			_fpsCounter += 10;
			_executeRenderTick();

			if (counterTimer.isRunning() == false)
			{
				_upsCounter.save();
				_fpsCounter.save();
				counterTimer.start();
			}
		}
	}

	void Application::_monothreadProcess()
	{
		spk::Timer counterTimer(100);

		while (_isRunning == true)
		{
			_fpsCounter += 10;
			_upsCounter += 10;

			_executeRenderTick();
			_executeUpdateTick();

			if (counterTimer.isRunning() == false)
			{
				_upsCounter.save();
				_fpsCounter.save();
				counterTimer.start();
			}
		}
	}

	Application::Application(const std::wstring& p_title, const spk::Vector2Int& p_size, bool p_isMonothread) :
		_errorID(0),
		_isRunning(false),
		_isMonothread(p_isMonothread),
		_APIModule(),
		_timeModule(),
		_systemModule(_APIModule.systemQueue(), this),
		_GAPIM(_APIModule.windowQueue(), p_title, p_size, &_APIModule),
		_mouseModule(_APIModule.mouseQueue()),
		_keyboardModule(_APIModule.keyboardQueue()),
		_widgetModule(),
		_profilerModule(),
		_fpsCounter(_profilerModule.profiler().metrics<Counter>(L"FPS Counter")),
		_upsCounter(_profilerModule.profiler().metrics<Counter>(L"UPS Counter"))
	{
		setAsInstance();
	}

	Application::~Application()
	{
		
	}

	int Application::run()
	{
		_initialisationProcess();

		if (_isMonothread == true)
		{
			_monothreadProcess();
		}
		else
		{
			spk::Thread updaterThread(L"Updater", [&](){_updateProcess();});

			_renderProcess();
		}

		return (_errorID);
	}

	void Application::quit(int p_errorID)
	{
		_errorID = p_errorID;
		_isRunning = false;
	}

	void Application::setKeyboardLayout(const spk::Keyboard::Layout& p_layout)
	{
		_keyboardModule.setKeyboardLayout(p_layout);
	}

	spk::Profiler& Application::profiler()
	{
		return (_profilerModule.profiler());
	}

	spk::Window& Application::window()
	{
		return (_GAPIM.window());
	}

	const spk::Vector2UInt& Application::size() const
	{
		return (_GAPIM.window().size());
	}
	
	const spk::Mouse& Application::mouse() const
	{	
		return (_mouseModule.mouse());
	}
	
	const spk::Keyboard& Application::keyboard() const
	{
		return (_keyboardModule.keyboard());
	}
}
