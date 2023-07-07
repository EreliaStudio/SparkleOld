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

		Contract _profilerModule_increaseRenderIPS_Contract;
		Contract _APIModule_update_Contract;
		Contract _windowModule_clear_Contract;
		Contract _widgetModule_render_Contract;
		Contract _windowModule_render_Contract;

		Contract _profilerModule_increaseUpdateIPS_Contract;
		Contract _systemModule_update_Contract;
		Contract _timeModule_update_Contract;
		Contract _profilerModule_update_Contract;
		Contract _windowModule_update_Contract;
		Contract _mouseModule_update_Contract;
		Contract _keyboardModule_update_Contract;
		Contract _widgetModule_update_Contract;
		Contract _mouseModule_updateMouse_Contract;
		Contract _keyboardModule_updateKeyboard_Contract;

	protected:
		/**
		 * @brief Setup the application jobs.
		 * 
		 * This method sets up the jobs for updating and rendering the modules.
		 */
		void setupJobs()
		{
			_profilerModule_increaseRenderIPS_Contract = addJob([&](){ _profilerModule->increaseRenderIPS();});
			_APIModule_update_Contract = addJob([&](){ _APIModule->update(); });
			_windowModule_clear_Contract = addJob([&](){ _windowModule->clear(); });
			_widgetModule_render_Contract = addJob([&](){ _widgetModule->render(); });
			_windowModule_render_Contract = addJob([&](){ _windowModule->render(); });

			_profilerModule_increaseUpdateIPS_Contract = addJob(L"Updater", [&](){ _profilerModule->increaseUpdateIPS();});
			_systemModule_update_Contract = addJob(L"Updater", [&](){ _systemModule->update(); });
			_timeModule_update_Contract = addJob(L"Updater", [&](){ _timeModule->update(); });
			_profilerModule_update_Contract = addJob(L"Updater", [&](){ _profilerModule->update();});
			_windowModule_update_Contract = addJob(L"Updater", [&](){ _windowModule->update(); });
			_mouseModule_update_Contract = addJob(L"Updater", [&](){ _mouseModule->update(); });
			_keyboardModule_update_Contract = addJob(L"Updater", [&](){ _keyboardModule->update(); });
			_widgetModule_update_Contract = addJob(L"Updater", [&](){ _widgetModule->update(); });
			_mouseModule_updateMouse_Contract = addJob(L"Updater", [&](){ _mouseModule->updateMouse(); });
			_keyboardModule_updateKeyboard_Contract = addJob(L"Updater", [&](){ _keyboardModule->updateKeyboard(); });

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
			result->setDepth(_widgetModule->centralWidget()->depth() + 1);

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