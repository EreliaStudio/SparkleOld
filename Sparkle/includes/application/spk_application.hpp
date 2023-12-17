#pragma once

#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_system_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_graphical_api_module.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"
#include "application/modules/spk_widget_module.hpp"
#include "application/modules/spk_profiler_module.hpp"

#include "widget/spk_widget.hpp"
#include "widget/spk_widget_canvas.hpp"

namespace spk
{
	/**
     * @class Application
     * @brief The central class of the application framework.
     *
     * Application serves as the core of the application framework, managing the main
     * application lifecycle, various modules, and system resources. It provides functionalities
     * for initializing modules, handling events, and rendering processes. Application acts as a
     * singleton, maintaining a thread-local instance accessible throughout the application.
     *
     * This class handles various aspects such as window management, input handling, widget
     * management, and profiling. It allows the creation and management of custom widgets and
     * interfaces, as well as providing access to system-level functionalities like keyboard and mouse input.
     *
     * The Application class supports both monothreaded and multithreaded operation modes, 
     * providing flexibility for different types of applications.
     */
	class Application
	{
		friend class Widget;
		friend class GraphicalAPIModule;

	public:
        thread_local static inline Application* _instance = nullptr; ///< Thread-local instance of the application.

        /**
         * @brief Retrieves the instance of the application.
         * @return Application instance.
         */
        static Application* instance() { return (_instance); }

        /**
         * @brief Sets the current object as the application instance.
         */
        void setAsInstance() { _instance = this; }

	private:
        bool _isRunning = false; ///< Indicates if the application is running.
        bool _isMonothread = false; ///< Flag for monothreaded operation.
        int _errorID = 0; ///< Error ID for tracking application errors.

		spk::APIModule _APIModule; ///< API module instance.

		spk::SystemModule _systemModule; ///< System module instance.
		spk::TimeModule _timeModule; ///< Time module instance.
		spk::GraphicalAPIModule _GAPIM; ///< Window module instance.
		spk::MouseModule _mouseModule; ///< Mouse module instance.
		spk::KeyboardModule _keyboardModule; ///< Keyboard module instance.

        spk::ProfilerModule _profilerModule; ///< Profiler module instance.
        Counter& _fpsCounter; ///< Frame per second counter.
        Counter& _upsCounter; ///< Updates per second counter.

		spk::WidgetModule _widgetModule; ///< Widget module instance.

		void _initialisationProcess();

		void _updateProcess();
		void _executeUpdateTick();
		
		void _renderProcess();
		void _executeRenderTick();

		void _monothreadProcess();

		WidgetModule::CentralWidget* _centralWidget();
		void _applyResizeOperation();

	public:
		/**
         * @brief Constructs an Application instance.
         * @param p_title Title of the application window.
         * @param p_size Size of the application window.
         * @param p_isMonothread Flag indicating if the application should run in monothreaded mode.
         */
        Application(const std::wstring& p_title, const spk::Vector2Int& p_size, bool p_isMonothread = false);

        /**
         * @brief Destructor of the Application class.
         */
        ~Application();

		/**
         * @brief Starts the application run loop.
         * @return Error ID indicating the status at the end of the application run.
         */
        int run();

        /**
         * @brief Signals the application to quit with a specified error ID.
         * @param p_errorID The error ID to quit with.
         */
        void quit(int p_errorID);

		/**
		 * @brief Accessor for the profiler module of the application.
		 * @return Reference to the Profiler module.
		 */
		spk::Profiler& profiler();

		/**
		 * @brief Accessor for the window module of the application.
		 * @return Reference to the Window module.
		 */
		spk::Window& window();

		spk::Widget* searchWidget(const std::wstring& p_name) const;

		/**
		 * @brief Gets the size of the application window.
		 * @return Constant reference to a Vector2UInt representing the size.
		 */
		const spk::Vector2UInt& size() const;

		/**
		 * @brief Accessor for the mouse module of the application.
		 * @return Constant reference to the Mouse module.
		 */
		const spk::Mouse& mouse() const;

		/**
		 * @brief Accessor for the keyboard module of the application.
		 * @return Constant reference to the Keyboard module.
		 */
		const spk::Keyboard& keyboard() const;
	};	
}
