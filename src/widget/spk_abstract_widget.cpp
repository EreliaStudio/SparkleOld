#include "widget/spk_abstract_widget.hpp"
#include "widget/spk_widget_atlas.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	void AbstractWidget::addChild(Child child)
	{
		InherenceObject<AbstractWidget>::addChild(child);
	}

	void AbstractWidget::_render()
	{
		if (isActive() == false)
			return ;
			
		if (_geometryEdited == true)
		{
			_onGeometryChange();
			_geometryEdited = false;
		}
			
		_onRender();
	}

	bool AbstractWidget::_update()
	{
		if (isActive() == false)
			return false;
		
		return (_onUpdate());
	}

	void AbstractWidget::_setOperationnal()
	{
		_isOperationnal = isActive() && (parent() == nullptr ? true : parent()->_isOperationnal);

		for (auto children : childrens())
		{
			children->_setOperationnal();
		}
	}

	AbstractWidget::AbstractWidget(const std::wstring& p_name) :
		_name(p_name)
	{
		WidgetAtlas::instance()->insert(this);

		_activationCallback = ActivableObject::addActivationCallback([&](){
			_setOperationnal();
		});

		_deactivationCallback = ActivableObject::addDeactivationCallback([&](){
			_setOperationnal();
		});
	}

	AbstractWidget::~AbstractWidget()
	{
		for (auto child : childrens())
		{
			delete child;
		}	
		WidgetAtlas::instance()->remove(this);
	}

	void AbstractWidget::setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2Int& p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		
		_geometryEdited = true;
	}
	
	void AbstractWidget::place(const spk::Vector2Int& p_anchor)
	{
		_anchor = p_anchor;
		
		_geometryEdited = true;
	}
	
	void AbstractWidget::move(const spk::Vector2Int& p_anchor)
	{
		_anchor += p_anchor;
		
		_geometryEdited = true;
	}

	void AbstractWidget::resize(const spk::Vector2Int& p_size)
	{
		_size = p_size;
		
		_geometryEdited = true;
	}

	void AbstractWidget::setDepth(const float& p_depth)
	{
		float delta(p_depth - _depth);

		_depth = p_depth;
		WidgetAtlas::instance()->sort(this);
		
		for (auto child : childrens())
		{
			child->setDepth(child->depth() + delta);
		}
	}

	void AbstractWidget::rename(const std::wstring& p_name)
	{
		_name = p_name;
	}

	bool AbstractWidget::isPointed(const spk::Vector2Int& p_point)
	{
		return (spk::Vector2Int::isInsideRectangle(p_point ,_anchor, _anchor + _size));
	}
};