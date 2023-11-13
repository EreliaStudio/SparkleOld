#include "graphics/viewport/spk_abstract_viewport.hpp"

namespace spk
{
	Area AbstractViewport::_computeActiveAbstractViewport() const
	{
		Area currentArea = _area;

		AbstractViewport* p_currentParent = parent();
		while (p_currentParent != nullptr)
		{
			Area parentArea = p_currentParent->_area;

			Vector2Int constrainedAnchor = currentArea.anchor() + parentArea.anchor();

			Vector2UInt constrainedSize = Vector2UInt::min(currentArea.size(), parentArea.size() - (constrainedAnchor - parentArea.anchor()));

			currentArea.setAnchor(constrainedAnchor);
			currentArea.setSize(constrainedSize);

			p_currentParent = p_currentParent->parent();
		}

		return currentArea;
	}

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
		_computedArea = _computeActiveAbstractViewport();
		_needComputation = false;
	}

	void AbstractViewport::activate() const
	{
		_onActivation(_computedArea);
		_activeViewport = _computedArea;
	}

	bool AbstractViewport::isInside(const Vector2Int& p_position)
	{
		return (_computeActiveAbstractViewport().isInside(p_position));
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
			2.0f * static_cast<float>(p_screenPosition.x) / static_cast<float>(_activeViewport.size().x) - 1.0f,
			(2.0f * static_cast<float>(p_screenPosition.y) / static_cast<float>(_activeViewport.size().y) - 1.0f) * -1
		);
	}

    float AbstractViewport::convertDepth(float p_screenDepth)
	{
		return (1 - (p_screenDepth / _maxDepth));
	}
}