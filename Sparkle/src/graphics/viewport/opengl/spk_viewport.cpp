#include "graphics/spk_viewport.hpp"
#include "graphics/spk_window.hpp"

namespace spk
{
	void Viewport::_onActivation(const spk::Area& p_area) const
	{
	//	spk::cout << "Viewport activation values : " << p_area << std::endl;
		glViewport(p_area.anchor().x, spk::Window::instance()->size().y - (p_area.size().y + p_area.anchor().y), p_area.size().x, p_area.size().y);
	}

	Viewport::Viewport() :
		spk::AbstractViewport()
	{

	}
}