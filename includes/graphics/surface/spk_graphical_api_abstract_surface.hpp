#pragma once

#include <string>
#include "math/spk_vector2.hpp"
#include "graphics/frame/spk_graphical_api_abstract_frame.hpp"

namespace spk::GraphicalAPI
{
	class AbstractSurface
	{
	protected:
		const spk::GraphicalAPI::AbstractFrame* _frame;

	public:
		AbstractSurface(const spk::GraphicalAPI::AbstractFrame* p_frame) :
			_frame(p_frame)
		{

		}

		virtual void resize() = 0;
		virtual void clear() = 0;
		virtual void render() = 0;
	};
}