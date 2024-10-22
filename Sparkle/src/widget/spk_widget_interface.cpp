#include "widget/spk_widget_interface.hpp"
#include "widget/spk_widget_atlas.hpp"
#include "application/spk_application.hpp"
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
		MONITOR_TIME_CONSUMPTION(_timeConsomptionMetrics);

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
		_name(p_name),
		_timeConsomptionMetrics(spk::Application::instance()->profiler().metrics<TimeConsumption>(p_name))
	{
		Atlas::instance()->insert(this);

		setBirthCallback([&](Parent p_parent){
			_viewport.setParent(&(p_parent->viewport()));
		});

		_activationCallback = ActivableObject::addActivationCallback([&](){
			_setOperationnal();
		});

		_deactivationCallback = ActivableObject::addDeactivationCallback([&](){
			_setOperationnal();
		});
	}

	Interface::Interface(const std::wstring& p_name, const spk::JSON::Object& p_inputObject) :
		Interface(p_name)
	{
		if (p_inputObject.contains(L"Parent") == true && p_inputObject[L"Parent"].hold<std::wstring>() == true)
		{
			std::wstring parentName = p_inputObject[L"Parent"].as<std::wstring>();
			Interface* parentWidget = spk::Widget::Atlas::instance()->get(parentName);
			if (parentWidget == nullptr)
			{
				spk::throwException(L"Widget [" + p_name + L"] : Parent [" + parentName + L"] does not exist");
			}
			setParent(parentWidget);
		}
		if (p_inputObject.contains(L"Activation") == true)
		{
			bool activationState = p_inputObject[L"Activation"].as<bool>();
			setState(activationState);
		}
	}

	Interface::~Interface()
	{
		for (auto child : childrens())
		{
			delete child;
		}	
		Atlas::instance()->remove(this);
	}

	void Interface::_computeResizeRatio()
	{
		const spk::Vector2UInt& areaSize = (parent() == nullptr ? spk::Application::instance()->window().size() : parent()->viewport().area().size());
		
		_anchorRatio = spk::Vector2(
			(anchor().x != 0 ? anchor().x / static_cast<float>(areaSize.x) : 0),
			(anchor().y != 0 ? anchor().y / static_cast<float>(areaSize.y) : 0)
		);

		_sizeRatio = spk::Vector2(
			(size().x != 0 ? size().x / static_cast<float>(areaSize.x) : 0),
			(size().y != 0 ? size().y / static_cast<float>(areaSize.y) : 0)
		);
	}

	void Interface::_applyResizeOperation()
	{
		const spk::Vector2UInt& areaSize = (parent() == nullptr ? spk::Application::instance()->window().size() : parent()->viewport().area().size());
		
		_viewport.setGeometry(spk::Area(areaSize * _anchorRatio, areaSize * _sizeRatio));
		_geometryEdited = true;
	}

	void Interface::setAnchor(const spk::Vector2Int& p_anchor)
	{
		_viewport.setAnchor(p_anchor);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}

	void Interface::setSize(const spk::Vector2UInt& p_size)
	{
		_viewport.setSize(p_size);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}

	void Interface::setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size)
	{
		_viewport.setGeometry(spk::Area(p_anchor, p_size));

		_computeResizeRatio();
		
		_geometryEdited = true;
	}
	
	void Interface::setGeometry(const spk::Area& p_area)
	{
		_viewport.setGeometry(p_area);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}
	
	void Interface::place(const spk::Vector2Int& p_anchor)
	{
		_viewport.setAnchor(p_anchor);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}
	
	void Interface::move(const spk::Vector2Int& p_delta)
	{
		_viewport.setAnchor(_viewport.area().anchor() + p_delta);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}

	void Interface::resize(const spk::Vector2UInt& p_size)
	{
		_viewport.setSize(p_size);
		
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
		return (_viewport.isInside(p_point));
	}
};