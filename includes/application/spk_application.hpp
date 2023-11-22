#pragma once

#include <map>
#include <vector>
#include <functional>
#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_system_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_graphical_api_module.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"
#include "application/modules/spk_widget_module.hpp"
#include "application/modules/spk_profiler_module.hpp"
#include "widget/spk_widget_canvas.hpp"

namespace spk
{
	/**
	 * @class Application
	 * @brief Abstract base class for the application.
	 *
	 * This class provides a skeletal implementation of an application, leaving the setup of jobs to subclasses.
	 */
	class Application
	{
	private:
		const std::function<void()> _updaterJob;

		spk::APIModule _APIModule; ///< API module instance.

		spk::SystemModule _systemModule; ///< System module instance.
		spk::TimeModule _timeModule; ///< Time module instance.
		spk::GraphicalAPIModule _GAPIM; ///< Window module instance.
		spk::MouseModule _mouseModule; ///< Mouse module instance.
		spk::KeyboardModule _keyboardModule; ///< Keyboard module instance.
		spk::ProfilerModule _profilerModule; ///< Profiler module instance.

		spk::WidgetModule _widgetModule; ///< Widget module instance.

		/**
		 * @brief Error code representing the state of the application.
		 */
		int _errorCode;
		
		/**
		 * @brief Boolean flag indicating if the application is running.
		 */
		bool _isRunning;

	protected:
		void _renameThread(const std::wstring& p_threadName);

		bool isRunning() const;

	public:
		/**
		 * @brief Constructs the Application object.
		 */
		Application(const std::wstring& p_title, const spk::Vector2Int& p_size);

		/**
		 * @brief Destructs the Application object.
		 */
		virtual ~Application();

		/**
		 * @brief Runs the application.
		 *
		 * @return The error code after the application finishes running.
		 */
		int run();

		/**
		 * @brief Quits the application with a specific error code.
		 *
		 * @param p_errorCode The error code with which the application should quit.
		 */
		void quit(int p_errorCode);

		

		/**
		 * @brief Load a new canvas into the application.
		 * @param p_configurationFilePath The path to the canvas configuration file.
		 * @return The newly created canvas widget.
		*/
		std::shared_ptr<spk::Widget::Canvas> addCanvas(const std::filesystem::path& p_configurationFilePath);

		/**
		 * @brief Add a new widget inside the application, with the central widget as parent.
		 *
		 * @tparam TChildrenType The type of the children widget.
		 * @tparam Args The types of the arguments for constructing the children widget.
		 * @param p_args The arguments for constructing the children widget.
		 * @return Pointer to the children widget.
		 */
		template <typename TChildrenType, typename ... Args>
		std::shared_ptr<TChildrenType> addRootWidget(Args&& ... p_args)
		{
			auto result = std::make_shared<TChildrenType>(std::forward<Args>(p_args)...);

			result->setDepth(0);

			return (result);
		}

		/**
		 * @brief Resizes the application window and central widget.
		 *
		 * @param p_size The new size of the window and central widget.
		 */
		void resize(const spk::Vector2Int& p_size);

		/**
		 * @brief Return the current size of the Window.
		*/
		const spk::Vector2UInt& size() const;

		void setMaxFPS(const size_t& p_maxFPS);
	};
}
