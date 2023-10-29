#include "graphics/viewport/spk_abstract_viewport.hpp"

namespace spk
{
	spk::Area AbstractViewport::_computeActiveAbstractViewport()
	{
		spk::Area currentArea = _area;

		AbstractViewport* p_currentParent = parent();
		while (p_currentParent != nullptr)
		{
			spk::Area parentArea = p_currentParent->_area;

			spk::Vector2Int constrainedAnchor = spk::Vector2Int::min(spk::Vector2Int::max(currentArea.anchor(), parentArea.anchor()), parentArea.anchor() + parentArea.size());

			spk::Vector2UInt constrainedSize = spk::Vector2UInt::min(currentArea.size(), parentArea.size() - (constrainedAnchor - parentArea.anchor()));

			currentArea.setAnchor(constrainedAnchor);
			currentArea.setSize(constrainedSize);

			p_currentParent = p_currentParent->parent();
		}

		return currentArea;
	}

	AbstractViewport::AbstractViewport() : _area()
	{
		
	}

	constexpr const spk::Area& AbstractViewport::area() const
	{
		return _area;
	}

	void AbstractViewport::setGeometry(const spk::Area& p_area)
	{
		_area = p_area;
	}

	void AbstractViewport::activate()
	{
		_activeViewport = _computeActiveAbstractViewport();

		_onActivation(_activeViewport);
	}

	spk::Vector2 AbstractViewport::convertScreenToGPUCoordinates(const spk::Vector2Int& p_screenPosition)
	{
		return spk::Vector2(
			2.0f * static_cast<float>(p_screenPosition.x) / static_cast<float>(_activeViewport.size().x) - 1.0f,
			2.0f * static_cast<float>(p_screenPosition.y) / static_cast<float>(_activeViewport.size().y) - 1.0f
		);
	}
}