#include "graphics/viewport/spk_abstract_viewport.hpp"

namespace spk
{
	AbstractViewport::AbstractViewport() : _area()
	{
		
	}

	void AbstractViewport::setGeometry(const Area& p_area)
	{
		_area = p_area;
		_needComputation = true;
	}

	void AbstractViewport::setAnchor(const Vector2Int& p_anchor)
	{
		_area.setAnchor(p_anchor);
		_needComputation = true;
	}
	
	void AbstractViewport::setSize(const Vector2UInt& p_size)
	{
		_area.setSize(p_size);
		_needComputation = true;
	}

	bool AbstractViewport::needComputation()
	{
		return (_needComputation);
	}
	
	void AbstractViewport::compute()
	{
		spk::Vector2Int cumulatedAnchor = _area.anchor();

		for (AbstractViewport* currentParent = parent(); currentParent != nullptr; currentParent = currentParent->parent())
		{
			cumulatedAnchor += currentParent->area().anchor();
		}

		const spk::Area& parentComputedArea = (parent() != nullptr ? parent()->_computedArea : _activeViewport);

		_computedArea.setAnchor(spk::Vector2Int::max(parentComputedArea.anchor(), cumulatedAnchor));
		_computedArea.setSize(spk::Vector2Int::min(parentComputedArea.size(), _area.size() + cumulatedAnchor));

		_activeViewportAnchorOffset = cumulatedAnchor - _computedArea.anchor();

		_needComputation = false;
	}

	void AbstractViewport::activate() const
	{
		_onActivation(_computedArea);
		_activeViewport = _computedArea;
	}

	bool AbstractViewport::isInside(const Vector2Int& p_position)
	{
		return (_activeViewport.isInside(p_position));
	}

	const spk::Area& AbstractViewport::activeViewport()
	{
		return (_activeViewport);
	}
	
	void AbstractViewport::resetViewport(const spk::Vector2UInt& p_baseViewportSize)
	{
		_activeViewport.setAnchor(0);
		_activeViewport.setSize(p_baseViewportSize);
	}

	Vector2 AbstractViewport::convertScreenToGPUCoordinates(const Vector2Int& p_screenPosition)
	{
		return Vector2(
			2.0f * static_cast<float>(p_screenPosition.x + _activeViewportAnchorOffset.x) / static_cast<float>(_activeViewport.size().x) - 1.0f,
			(2.0f * static_cast<float>(p_screenPosition.y + _activeViewportAnchorOffset.x) / static_cast<float>(_activeViewport.size().y) - 1.0f) * -1
		);
	}

    float AbstractViewport::convertDepth(float p_screenDepth)
	{
		return (1 - (p_screenDepth / _maxDepth));
	}
}