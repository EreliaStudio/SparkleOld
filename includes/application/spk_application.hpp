#pragma once 

#include "application/spk_abstract_application.hpp"
#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_system_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_graphical_api_module.hpp"
#include "graphics/spk_window.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"
#include "application/modules/spk_widget_module.hpp"
#include "application/modules/spk_profiler_module.hpp"
#include "design_pattern/spk_observer.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "widget/spk_widget_canvas.hpp"

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
		spk::APIModule _APIModule; ///< API module instance.

		spk::SystemModule _systemModule; ///< System module instance.
		spk::TimeModule _timeModule; ///< Time module instance.
		spk::GraphicalAPIModule _GAPIM; ///< Window module instance.
		spk::MouseModule _mouseModule; ///< Mouse module instance.
		spk::KeyboardModule _keyboardModule; ///< Keyboard module instance.
		spk::ProfilerModule _profilerModule; ///< Profiler module instance.

		spk::WidgetModule _widgetModule; ///< Widget module instance.

		std::vector<Contract> _renderContracts; ///< Contracts related to the Render jobs
		std::vector<Contract> _updateContracts; ///< Contracts related to the Update jobs

	protected:
		/**
		 * @brief Setup the application jobs.
		 *
		 * This method sets up the jobs for updating and rendering the modules.
		 */
		void setupJobs();

	public:
		/**
		 * @brief Constructs the Application object.
		 *
		 * @param p_title Title of the window.
		 * @param p_size Size of the window.
		 */
		Application(const std::wstring& p_title, const spk::Vector2Int& p_size);

		/**
		 * @brief Destructs the Application object.
		 *
		 * This will delete all modules.
		 */
		~Application();

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

			result->setDepth(1);

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
	};

}
