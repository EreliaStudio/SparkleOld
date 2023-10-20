#pragma once

#include "graphics/spk_frame.hpp"
#include "graphics/spk_surface.hpp"

namespace spk
{
	class Window : public spk::Singleton<Window>
	{
		friend class spk::Singleton<Window>;

	private:
		spk::Frame _frame;
		spk::Surface _surface;

		Window(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr = nullptr) :
			_frame(p_title, p_size, p_ptr),
			_surface(&_frame)
		{

		}
	public:

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&&) = default;
		Window& operator=(Window&&) = default;

		void clear()
		{
			_surface.clear();
			_frame.clear();
		}

		void render()
		{
			_surface.render();
			_frame.render();
		}

		void setSize(const spk::Vector2UInt& p_size)
		{
			_frame.setSize(p_size);
			_surface.resize();
		}

		void resize(const spk::Vector2UInt& p_size)
		{
			_frame.resize(p_size);
			_surface.resize();
		}

		const spk::Vector2UInt& size() const 
		{
			return (_frame.size());
		}

		const spk::Frame& frame() const
		{
			return (_frame);
		}
	};
}