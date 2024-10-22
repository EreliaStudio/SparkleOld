#include "graphics/spk_window.hpp"
#include "graphics/spk_viewport.hpp"

namespace spk
{
	Window::Window(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr) :
		_frame(p_title, p_size, p_ptr),
		_surface(&_frame)
	{

	}

	void Window::clear()
	{
		_surface.clear();
		_frame.clear();
	}

	void Window::render()
	{
		_surface.render();
		_frame.render();
	}

	void Window::setSize(const spk::Vector2UInt& p_size)
	{
		_frame.setSize(p_size);
		_surface.resize();
	}

	void Window::resize(const spk::Vector2UInt& p_size)
	{
		_frame.resize(p_size);
		_surface.resize();
	}

	void Window::place(const spk::Vector2Int& p_anchor)
	{
		_frame.place(p_anchor);
	}

    const spk::Vector2Int& Window::anchor() const
	{
		return (_frame.anchor());
	}

	const spk::Vector2UInt& Window::size() const 
	{
		return (_frame.size());
	}

	const spk::Frame& Window::frame() const
	{
		return (_frame);
	}
}