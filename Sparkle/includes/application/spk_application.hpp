#pragma once

#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_system_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_graphical_api_module.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"
#include "application/modules/spk_widget_module.hpp"
#include "application/modules/spk_profiler_module.hpp"

#include "widget/spk_widget_interface.hpp"
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
         * @brief Adds a canvas widget to the application.
         * @param p_configurationFilePath File path for the canvas configuration.
         * @return Shared pointer to the created Canvas widget.
         */
		std::shared_ptr<spk::Widget::Canvas> addCanvas(const std::filesystem::path& p_configurationFilePath)
		{
			auto result = std::make_shared<spk::Widget::Canvas>(p_configurationFilePath);

			result->setGeometry(spk::Vector2Int(0, 0), _GAPIM.window().size());

			return (result);
		}

		/**
		 * @brief Creates and adds a root widget of type TChildrenType to the application.
		 * 
		 * This function template creates a widget of the specified type using the provided arguments.
		 * The created widget is set as a root widget with a depth of 0.
		 * 
		 * @tparam TChildrenType The type of the widget to be created.
		 * @tparam Args Variadic template for constructor arguments of TChildrenType.
		 * @param p_args Arguments to be forwarded to the constructor of TChildrenType.
		 * @return std::shared_ptr<TChildrenType> Shared pointer to the newly created widget.
		 */
		template <typename TChildrenType, typename ... Args>
		std::shared_ptr<TChildrenType> addRootWidget(Args&& ... p_args)
		{
			auto result = std::make_shared<TChildrenType>(std::forward<Args>(p_args)...);

			result->setDepth(0);

			return (result);
		}

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
