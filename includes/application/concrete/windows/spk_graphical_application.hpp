#pragma once 

#include "application/spk_abstract_application.hpp"
#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_window_module.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"
#include "application/modules/spk_widget_module.hpp"

namespace spk
{
	class Application : public spk::AbstractApplication
	{
	private:
		spk::APIModule *_APIModule;
		spk::TimeModule *_timeModule;
		spk::WindowModule *_windowModule;
		spk::MouseModule *_mouseModule;
		spk::KeyboardModule *_keyboardModule;

		spk::WidgetModule* _widgetModule;

	protected:
		void setupJobs()
		{
			addJob([&]()
				{ _APIModule->update(); });

			addJob(L"Updater", [&]()
				{ _timeModule->update(); });

			addJob(L"Updater", [&]()
				{ _windowModule->update(); });

			addJob(L"Updater", [&]()
				{ _mouseModule->update(); });
			addJob(L"Updater", [&]()
				{ _keyboardModule->update(); });

			addJob(L"Updater", [&]()
				{ _widgetModule->update(); });

			addJob(L"Updater", [&]()
				{ _mouseModule->updateMouse(); });
			addJob(L"Updater", [&]()
				{ _keyboardModule->updateKeyboard(); });

			addJob([&]()
				{ _windowModule->clear(); });
			addJob([&]()
				{ _widgetModule->render(); });
			addJob([&]()
				{ _windowModule->render(); });
		}

	public:
		Application(spk::Vector2Int p_size)
		{
			_APIModule = new spk::APIModule();
			_timeModule = new spk::TimeModule();
			
			_windowModule = new spk::WindowModule(_APIModule->windowQueue(), p_size, _APIModule);
			
			_mouseModule = new spk::MouseModule(_APIModule->mouseQueue());
			_keyboardModule = new spk::KeyboardModule(_APIModule->keyboardQueue());

			_widgetModule = new spk::WidgetModule();
			_widgetModule->centralWidget()->setGeometry(0, p_size);
		}

		~Application()
		{
			delete _APIModule;
			delete _timeModule;
			delete _windowModule;
			delete _mouseModule;
			delete _keyboardModule;

			delete _widgetModule;
		}

		spk::AbstractWidget* centralWidget()
		{
			return (_widgetModule->centralWidget());
		}

		void resize(spk::Vector2Int p_size)
		{
			spk::Singleton<spk::Window>::instance()->setGeometry(p_size);
			_widgetModule->centralWidget()->setGeometry(0, p_size);
		}
	};
}