#pragma once

#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_system_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_graphical_api_module.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"
#include "application/modules/spk_widget_module.hpp"

#include "widget/spk_widget_interface.hpp"
#include "widget/spk_widget_canvas.hpp"

namespace spk
{
	class Application
	{
	public:
		static inline Application* _instance = nullptr;
		static Application* instance() { return (_instance); }
		void setAsInstance() { _instance = this; }

	private:
		bool _isRunning = false;
		int _errorID = 0;

		spk::APIModule _APIModule; ///< API module instance.

		spk::SystemModule _systemModule; ///< System module instance.
		spk::TimeModule _timeModule; ///< Time module instance.
		spk::GraphicalAPIModule _GAPIM; ///< Window module instance.
		spk::MouseModule _mouseModule; ///< Mouse module instance.
		spk::KeyboardModule _keyboardModule; ///< Keyboard module instance.

		spk::WidgetModule _widgetModule; ///< Widget module instance.

		void _initialisationProcess();

		void _updateProcess();
		void _executeUpdateTick();
		
		void _renderProcess();
		void _executeRenderTick();

	public:
		Application(const std::wstring& p_title, const spk::Vector2Int& p_size);
		~Application();

		std::shared_ptr<spk::Widget::Canvas> addCanvas(const std::filesystem::path& p_configurationFilePath)
		{
			auto result = std::make_shared<spk::Widget::Canvas>(p_configurationFilePath);

			result->setGeometry(spk::Vector2Int(0, 0), _GAPIM.window().size());

			return (result);
		}

		template <typename TChildrenType, typename ... Args>
		std::shared_ptr<TChildrenType> addRootWidget(Args&& ... p_args)
		{
			auto result = std::make_shared<TChildrenType>(std::forward<Args>(p_args)...);

			result->setDepth(0);

			return (result);
		}

		int run();

		void quit(int p_errorID);

		void setKeyboardLayout(const spk::Keyboard::Layout& p_layout);

		spk::Window& window();
		const spk::Vector2UInt& size() const;
		const spk::Mouse& mouse() const;
		const spk::Keyboard& keyboard() const;
	};	
}
