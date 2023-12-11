#include "application/spk_application.hpp"

#include "threading/spk_thread.hpp"

#include "engine/component/spk_mesh_renderer_component.hpp"

namespace spk
{
	void Application::_initialisationProcess()
	{
		_errorID = 0;
		_isRunning = true;

		MeshRenderer::initializeMeshRendererShader();
	}

	Application::Application(const std::wstring& p_title, const spk::Vector2Int& p_size) :
		_errorID(0),
		_isRunning(false),
		_APIModule(),
		_timeModule(),
		_systemModule(_APIModule.systemQueue(), this),
		_GAPIM(_APIModule.windowQueue(), p_title, p_size, &_APIModule),
		_mouseModule(_APIModule.mouseQueue()),
		_keyboardModule(_APIModule.keyboardQueue()),
		_widgetModule()
	{
		setAsInstance();
	}

	Application::~Application()
	{
		
	}

	int Application::run()
	{
		_initialisationProcess();

		spk::Thread updaterThread(L"Updater", [&](){_updateProcess();});

		_renderProcess();

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
