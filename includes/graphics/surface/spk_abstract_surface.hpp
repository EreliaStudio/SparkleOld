#pragma once

#include <string>
#include "math/spk_vector2.hpp"
#include "graphics/frame/spk_abstract_frame.hpp"

namespace spk
{
	class AbstractSurface
	{
	protected:
		const spk::AbstractFrame* _frame;

	public:
		AbstractSurface(const spk::AbstractFrame* p_frame) :
			_frame(p_frame)
		{

		}

		AbstractSurface(const AbstractSurface&) = delete;
		AbstractSurface& operator=(const AbstractSurface&) = delete;

		AbstractSurface(AbstractSurface&&) = default;
		AbstractSurface& operator=(AbstractSurface&&) = default;

		virtual void resize() = 0;
		virtual void clear() = 0;
		virtual void render() = 0;
	};
}