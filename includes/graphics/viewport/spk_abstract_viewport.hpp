#pragma once

#include "graphics/spk_area.hpp"
#include "design_pattern/spk_inherence_object.hpp"

namespace spk
{
	class AbstractViewport : public spk::InherenceObject<AbstractViewport>
	{
	private:
		static inline spk::Area _activeViewport;

		spk::Area _area;

		virtual void _onActivation(const spk::Area& p_area) = 0;

		spk::Area _computeActiveAbstractViewport();
	public:
		AbstractViewport();

		constexpr const spk::Area& area() const;
		void setGeometry(const spk::Area& p_area);

		void activate();

		static spk::Vector2 convertScreenToGPUCoordinates(const spk::Vector2Int& p_screenPosition);
	};

}