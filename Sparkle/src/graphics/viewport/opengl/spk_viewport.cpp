#include "graphics/spk_viewport.hpp"
#include "application/spk_application.hpp"

namespace spk
{
	void Viewport::_onActivation(const spk::Area& p_area) const
	{
	//	spk::cout << "Viewport activation values : " << p_area << std::endl;
		glViewport(p_area.anchor().x, spk::Application::instance()->window().size().y - (p_area.size().y + p_area.anchor().y), p_area.size().x, p_area.size().y);
	}

	Viewport::Viewport() :
		spk::AbstractViewport()
	{

	}
}