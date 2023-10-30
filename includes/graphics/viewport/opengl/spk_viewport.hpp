#pragma once

#include "graphics/viewport/spk_abstract_viewport.hpp"

namespace spk
{
	class Viewport : public spk::AbstractViewport
	{
	private:
		void _onActivation(const spk::Area& p_area) const;

	public:
		Viewport();
	};
}