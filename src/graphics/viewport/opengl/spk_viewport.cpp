#include "graphics/spk_viewport.hpp"

namespace spk
{
	void Viewport::_onActivation(const spk::Area& p_area) const
	{
		glViewport(p_area.anchor().x, activeViewport().size().y - (p_area.size().y + p_area.anchor().y), p_area.size().x, p_area.size().y);
	}

	Viewport::Viewport() :
		spk::AbstractViewport()
	{

	}
}