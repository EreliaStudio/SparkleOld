#pragma once

#include "graphics/spk_area.hpp"
#include "design_pattern/spk_inherence_object.hpp"

namespace spk
{
	class AbstractViewport : public spk::InherenceObject<AbstractViewport>
	{
	private:
		static inline Area _activeViewport;

		Area _area;

		virtual void _onActivation(const Area& p_area) const = 0;

		Area _computeActiveAbstractViewport() const;
	public:
		AbstractViewport();

		constexpr Area& area()
		{
			return (_area);
		}

		constexpr const Area& area() const
		{
			return (_area);
		}

		void setGeometry(const Area& p_area);
		void setAnchor(const spk::Vector2Int& p_anchor);
		void setSize(const spk::Vector2UInt& p_size);

		void activate() const;

		bool isInside(const spk::Vector2Int& p_position);

		static const spk::Area& activeViewport();
		static void resetViewport(const spk::Vector2UInt& p_baseViewportSize);
		static spk::Vector2 convertScreenToGPUCoordinates(const spk::Vector2Int& p_screenPosition);
	};

}