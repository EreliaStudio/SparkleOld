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

		Window(const std::wstring &p_title, const Vector2Int &p_size, void *p_ptr = nullptr) :
			_frame(p_title, p_size, p_ptr),
			_surface(p_size)
		{

		}

	public:

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