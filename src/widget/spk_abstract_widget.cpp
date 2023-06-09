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
		
		for (size_t i(0); i < childrens().size(); ++i)
		{
			childrens()[i]->_render();
		}
	}

	bool AbstractWidget::_update()
	{
		if (isActive() == false)
			return false;
		for (size_t i(0); i < childrens().size(); ++i)
			if (childrens()[i]->_update() == true)
				return (true);
		
		return (_onUpdate());
	}

	AbstractWidget::AbstractWidget(std::wstring p_name) :
		_name(p_name)
	{

	}

	AbstractWidget::~AbstractWidget()
	{
		for (size_t i(0); i < childrens().size(); ++i)
		{
			delete childrens()[i];
		}	
	}

	template <typename TChildrenType, typename ... Args>
	TChildrenType* AbstractWidget::addChildrenWidget(Args&& ... p_args)
	{
		TChildrenType * result = new TChildrenType(std::forward<Args>(p_args)...);

		addChild(result);
		result->setDepth(depth() + 1);

		return (result);
	}

	void AbstractWidget::setGeometry(spk::Vector2Int p_anchor, spk::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		
		_geometryEdited = true;
	}
	
	void AbstractWidget::place(spk::Vector2Int p_anchor)
	{
		_anchor = p_anchor;
		
		_geometryEdited = true;
	}
	
	void AbstractWidget::move(spk::Vector2Int p_anchor)
	{
		_anchor += p_anchor;
		
		_geometryEdited = true;
	}

	void AbstractWidget::resize(spk::Vector2Int p_size)
	{
		_size = p_size;
		
		_geometryEdited = true;
	}

	void AbstractWidget::sortChildrensByDepth()
	{
		std::sort(childrens().begin(), childrens().end(), [](AbstractWidget* lhs, AbstractWidget* rhs) {
					return lhs->depth() > rhs->depth();
				});
	}

	void AbstractWidget::setDepth(const float& p_depth)
	{
		float delta(p_depth - _depth);

		_depth = p_depth;

		for (size_t i(0); i < childrens().size(); ++i)
			childrens()[i]->setDepth(childrens()[i]->depth() + delta);
	}

	void AbstractWidget::rename(const std::wstring& p_name)
	{
		_name = p_name;
	}

	bool AbstractWidget::isPointed(spk::Vector2Int p_point)
	{
		return (spk::Vector2Int::isInsideRectangle(p_point ,_anchor, _anchor + _size));
	}
};