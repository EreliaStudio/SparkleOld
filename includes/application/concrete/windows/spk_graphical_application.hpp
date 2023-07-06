#pragma once 

#include "application/spk_abstract_application.hpp"
#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_window_module.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"
#include "application/modules/spk_widget_module.hpp"
#include "application/modules/spk_system_module.hpp"
#include "application/modules/spk_profiler_module.hpp"

namespace spk
{
	/**
	 * @class Application
	 * @brief Main class for managing application.
	 * 
	 * This class initializes the SPK modules and manages the application loop.
	 * The modules are updated and rendered in a specific order.
	 * The application window can be resized, and the central widget can be retrieved.
	 */
	class Application : public spk::AbstractApplication
	{
	private:
	
		spk::APIModule *_APIModule; ///< API module instance.
		spk::TimeModule *_timeModule; ///< Time module instance.
		spk::WindowModule *_windowModule; ///< Window module instance.
		spk::MouseModule *_mouseModule; ///< Mouse module instance.
		spk::KeyboardModule *_keyboardModule; ///< Keyboard module instance.
		spk::SystemModule *_systemModule; ///< System module instance.
		spk::ProfilerModule *_profilerModule; ///< Profiler module instance.

		spk::WidgetModule* _widgetModule; ///< Widget module instance.

	protected:
		/**
		 * @brief Setup the application jobs.
		 * 
		 * This method sets up the jobs for updating and rendering the modules.
		 */
		void setupJobs()
		{
			addJob([&](){ _profilerModule->increaseRenderIPS();});
			addJob([&](){ _APIModule->update(); });
			addJob([&](){ _windowModule->clear(); });
			addJob([&](){ _widgetModule->render(); });
			addJob([&](){ _windowModule->render(); });

			addJob(L"Updater", [&](){ _profilerModule->increaseUpdateIPS();});
			addJob(L"Updater", [&](){ _systemModule->update(); });
			addJob(L"Updater", [&](){ _timeModule->update(); });
			addJob(L"Updater", [&](){ _profilerModule->update();});

			addJob(L"Updater", [&](){ _windowModule->update(); });

			addJob(L"Updater", [&](){ _mouseModule->update(); });
			addJob(L"Updater", [&](){ _keyboardModule->update(); });

			addJob(L"Updater", [&](){ _widgetModule->update(); });

			addJob(L"Updater", [&](){ _mouseModule->updateMouse(); });
			addJob(L"Updater", [&](){ _keyboardModule->updateKeyboard(); });

		}

	public:
		/**
		 * @brief Constructs the Application object.
		 * 
		 * @param p_title Title of the window.
		 * @param p_size Size of the window.
		 */
		Application(const std::wstring& p_title, const spk::Vector2Int& p_size)
		{
			_APIModule = new spk::APIModule();
			
			_systemModule = new spk::SystemModule(_APIModule->systemQueue(), this);
			_timeModule = new spk::TimeModule();
			_profilerModule = new spk::ProfilerModule();
			
			_windowModule = new spk::WindowModule(_APIModule->windowQueue(), p_title, p_size, _APIModule);
			
			_mouseModule = new spk::MouseModule(_APIModule->mouseQueue());
			_keyboardModule = new spk::KeyboardModule(_APIModule->keyboardQueue());

			_widgetModule = new spk::WidgetModule();
			resize(p_size);
		}

		/**
		 * @brief Destructs the Application object.
		 * 
		 * This will delete all modules.
		 */
		~Application()
		{
			delete _APIModule;
			delete _timeModule;
			delete _windowModule;
			delete _mouseModule;
			delete _keyboardModule;
			delete _systemModule;
			delete _profilerModule;

			delete _widgetModule;
		}

		/**
		 * @brief Returns the central widget of the application.
		 * 
		 * @return The central widget of the application.
		 */
		spk::AbstractWidget* centralWidget()
		{
			return (_widgetModule->centralWidget());
		}

		/**
		 * @brief Add a new widget inside the application, with the central widget as parent.
		 * 
         * @tparam TChildrenType The type of the children widget.
         * @tparam Args The types of the arguments for constructing the children widget.
         * @param p_args The arguments for constructing the children widget.
         * @return Pointer to the children widget.
		 */
		template <typename TChildrenType, typename ... Args>
		TChildrenType* addRootWidget(Args&& ... p_args)
		{
			TChildrenType * result = new TChildrenType(std::forward<Args>(p_args)...);

			_widgetModule->centralWidget()->addChild(result);
			result->setDepth(depth() + 1);

			return (result);
		}


		/**
		 * @brief Resizes the application window and central widget.
		 * 
		 * @param p_size The new size of the window and central widget.
		 */
		void resize(const spk::Vector2Int& p_size)
		{
			spk::Singleton<spk::Window>::instance()->setGeometry(p_size);
			_widgetModule->centralWidget()->setGeometry(0, p_size);
		}
	};
}