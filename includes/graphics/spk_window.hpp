#pragma once

#include "graphics/spk_graphical_api_frame.hpp"
#include "graphics/spk_graphical_api_surface.hpp"

namespace spk
{
	class Window : public spk::Singleton<Window>
	{
		friend class spk::Singleton<Window>;

	private:
		spk::GraphicalAPI::Frame _frame;
		spk::GraphicalAPI::Surface _surface;

		Window(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr = nullptr) :
			_frame(p_title, p_size, p_ptr),
			_surface(p_size)
		{

		}
	public:

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&&) = default;
		Window& operator=(Window&&) = default;

		void clear()
		{
			_frame.clear();
			_surface.clear();
		}

		void render()
		{
			_frame.render();
			_surface.render();
		}

		void setSize(const spk::Vector2UInt& p_size)
		{
			_frame.setSize(p_size);
			_surface.resize(p_size);
		}

		void resize(const spk::Vector2UInt& p_size)
		{
			_frame.resize(p_size);
			_surface.resize(p_size);
		}

		const spk::Vector2UInt& size() const 
		{
			return (_frame.size());
		}

		const spk::GraphicalAPI::Frame& frame() const
		{
			return (_frame);
		}
	};
}