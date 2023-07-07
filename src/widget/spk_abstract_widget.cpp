#include "widget/spk_abstract_widget.hpp"

namespace spk
{
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

	AbstractWidget::AbstractWidget(const std::wstring& p_name) :
		_name(p_name)
	{
		WidgetAtlas::instance()->insert(this);
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
		WidgetAtlas::instance()->update(this);
		
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