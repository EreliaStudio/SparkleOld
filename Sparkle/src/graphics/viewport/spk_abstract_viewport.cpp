#include "graphics/viewport/spk_abstract_viewport.hpp"
#include "application/spk_application.hpp"

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
	
	spk::Vector2Int AbstractViewport::_computeAbsoluteAnchor()
	{
		spk::Vector2Int result = _area.anchor();

		for (AbstractViewport* currentParent = parent(); currentParent != nullptr; currentParent = currentParent->parent())
		{
			result += currentParent->area().anchor();
		}

		return (result);
	}

	void AbstractViewport::compute()
	{
		spk::Vector2Int targetTopLeft = _computeAbsoluteAnchor();
		spk::Vector2Int targetDownRight = targetTopLeft + _area.size();

		spk::Area parentActiveArea = (parent() != nullptr ? parent()->_computedArea : spk::Area(0, spk::Application::instance()->window().size()));
		spk::Vector2Int parentTopLeft = parentActiveArea.anchor();
		spk::Vector2Int parentDownRight = parentTopLeft + parentActiveArea.size();

		spk::Vector2Int resultTopLeft = spk::Vector2Int::max(targetTopLeft, parentTopLeft);
		spk::Vector2Int resultDownRight = spk::Vector2Int(
			std::min(targetDownRight.x, parentDownRight.x),
			std::min(targetDownRight.y, parentDownRight.y)
		);

		_computedArea.setAnchor(resultTopLeft);
		_computedArea.setSize(resultDownRight - resultTopLeft);
		_computedViewportAnchorOffset = targetTopLeft - resultTopLeft;

		_needComputation = false;
	}

	void AbstractViewport::activate() const
	{
		_onActivation(_computedArea);
		_activeViewport = _computedArea;
		_activeViewportAnchorOffset = _computedViewportAnchorOffset;
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
		_activeViewportAnchorOffset = 0;
	}

	Vector2 AbstractViewport::convertScreenToGPUCoordinates(const Vector2Int& p_screenPosition)
	{
		return Vector2(
			2.0f * static_cast<float>(p_screenPosition.x + _activeViewportAnchorOffset.x) / static_cast<float>(_activeViewport.size().x) - 1.0f,
			(2.0f * static_cast<float>(p_screenPosition.y + _activeViewportAnchorOffset.y) / static_cast<float>(_activeViewport.size().y) - 1.0f) * -1
		);
	}

    float AbstractViewport::convertDepth(float p_screenDepth)
	{
		return (1 - (p_screenDepth / _maxDepth));
	}
}