#include "widget/spk_widget_interface.hpp"
#include "widget/spk_widget_atlas.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk::Widget
{
	void Interface::addChild(Child child)
	{
		InherenceObject<Interface>::addChild(child);
	}

	void Interface::_render()
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

	bool Interface::_update()
	{
		if (isActive() == false)
			return false;
		
		return (_onUpdate());
	}

	void Interface::_setOperationnal()
	{
		_isOperationnal = isActive() && (parent() == nullptr ? true : parent()->_isOperationnal);

		for (auto children : childrens())
		{
			children->_setOperationnal();
		}
	}

	Interface::Interface(const std::wstring& p_name) :
		_name(p_name)
	{
		Atlas::instance()->insert(this);

		_activationCallback = ActivableObject::addActivationCallback([&](){
			_setOperationnal();
		});

		_deactivationCallback = ActivableObject::addDeactivationCallback([&](){
			_setOperationnal();
		});
	}

	Interface::~Interface()
	{
		for (auto child : childrens())
		{
			delete child;
		}	
		Atlas::instance()->remove(this);
	}

	void Interface::setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2Int& p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		
		_geometryEdited = true;
	}
	
	void Interface::place(const spk::Vector2Int& p_anchor)
	{
		_anchor = p_anchor;
		
		_geometryEdited = true;
	}
	
	void Interface::move(const spk::Vector2Int& p_anchor)
	{
		_anchor += p_anchor;
		
		_geometryEdited = true;
	}

	void Interface::resize(const spk::Vector2Int& p_size)
	{
		_size = p_size;
		
		_geometryEdited = true;
	}

	void Interface::setDepth(const float& p_depth)
	{
		float delta(p_depth - _depth);

		_depth = p_depth;
		Atlas::instance()->sort(this);
		
		for (auto child : childrens())
		{
			child->setDepth(child->depth() + delta);
		}
	}

	void Interface::rename(const std::wstring& p_name)
	{
		_name = p_name;
	}

	bool Interface::isPointed(const spk::Vector2Int& p_point)
	{
		return (spk::Vector2Int::isInsideRectangle(p_point ,_anchor, _anchor + _size));
	}
};