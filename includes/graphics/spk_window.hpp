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

		Window(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr = nullptr);
	public:

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&&) = default;
		Window& operator=(Window&&) = default;

		void clear();
		void render();

		void setSize(const spk::Vector2UInt& p_size);
		void resize(const spk::Vector2UInt& p_size);

		const spk::Vector2UInt& size() const;
		const spk::Frame& frame() const;
	};
}