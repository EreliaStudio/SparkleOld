#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"
#include "graphics/frame/spk_abstract_frame.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	class Frame : public spk::AbstractFrame
	{
		friend class APIModule;

	private:
		bool _wasResized = { false }; ///< Whether the window was resized.

		Display* _display;
		int _defaultScreen;

		xcb_connection_t* _connection;
		xcb_screen_t* _screen;
		xcb_window_t _window;
		xcb_intern_atom_reply_t* _atom_wm_delete_window;

		void _createFrame();

		void _nameFrame(const std::wstring& p_title);

		static xcb_intern_atom_reply_t* _enableDestroyXCBProperty(xcb_connection_t* p_connection, xcb_window_t p_window);

		void _onResize();
		void _onSetSize();

	public:
		Frame(const std::wstring& p_title, const spk::Vector2UInt& p_size, void* p_ptr = nullptr);

		~Frame();

		bool wasResized() const;

		void resizeHandled();

		const spk::Vector2UInt& size() const;

		void render();

		void clear();

		Display* display() const { return (_display); }
		int defaultScreen() const { return (_defaultScreen); }
		xcb_connection_t* connection() const { return (_connection); }
		xcb_window_t window() const { return (_window); }

		xcb_atom_t atomWmDeleteWindow() const { return (_atom_wm_delete_window->atom); }
	};
}
