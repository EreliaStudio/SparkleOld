#include "widget/spk_widget.hpp"
#include "application/spk_application.hpp"
#include "iostream/spk_iostream.hpp"

namespace spk
{
	void Widget::_render()
	{
		if (isActive() == false)
			return ;
			
		if (_geometryEdited == true)
		{
			_onGeometryChange();
			_geometryEdited = false;
		}
			
		_onRender();

		for (auto& child : childrens())
		{
			child->_render();
		}
	}

	bool Widget::_update()
	{

		if (isActive() == false)
			return false;

		for (auto& child : childrens())
		{
			if (child->_update() == true)
				return (true);
		}

		MONITOR_TIME_CONSUMPTION(_timeConsomptionMetrics);
		return (_onUpdate());
	}

	void Widget::_setOperationnal()
	{
		_isOperationnal = isActive() && (parent() == nullptr ? true : parent()->_isOperationnal);

		for (auto children : childrens())
		{
			children->_setOperationnal();
		}
	}

	Widget::Widget(const std::wstring& p_name) :
		_name(p_name),
		_timeConsomptionMetrics(spk::Application::instance()->profiler().metrics<TimeConsumption>(p_name))
	{
		if (Application::instance() != nullptr &&
			Application::instance()->_centralWidget() != nullptr &&
			Application::instance()->_centralWidget() != this)
		{
			Application::instance()->_centralWidget()->addChild(this);
		}

		setLinkingCallback([&](Widget* p_parent){
			Viewport* parentViewport = &(p_parent->viewport());
			parentViewport->addChild(&_viewport);
		});

		_activationContract = ActivableObject::addActivationCallback([&](){
			_setOperationnal();
		});

		_deactivationContract = ActivableObject::addDeactivationCallback([&](){
			_setOperationnal();
		});
	}

	Widget::Widget(const std::wstring& p_name, const spk::JSON::Object& p_inputObject) :
		Widget(p_name)
	{
		if (p_inputObject.contains(L"Parent") == true && p_inputObject[L"Parent"].hold<std::wstring>() == true)
		{
			std::wstring parentName = p_inputObject[L"Parent"].as<std::wstring>();
			Widget* parentWidget = spk::Application::instance()->searchWidget(parentName);
			if (parentWidget == nullptr)
			{
				spk::throwException(L"Widget [" + p_name + L"] : Parent [" + parentName + L"] does not exist");
			}
			parentWidget->addChild(this);
		}
		if (p_inputObject.contains(L"Activation") == true)
		{
			bool activationState = p_inputObject[L"Activation"].as<bool>();
			if (activationState == true)
				activate();
			else
				deactivate();
		}
	}

	Widget::Widget(Widget&& p_other) noexcept :
		_isOperationnal(std::move(p_other._isOperationnal)),
		_activationContract(std::move(p_other._activationContract)),
		_deactivationContract(std::move(p_other._deactivationContract)),
		_timeConsomptionMetrics(p_other._timeConsomptionMetrics),
		_name(std::move(p_other._name)),
		_geometryEdited(std::move(p_other._geometryEdited)),
		_depth(std::move(p_other._depth)),
		_viewport(std::move(p_other._viewport)),
		_anchorRatio(std::move(p_other._anchorRatio)),
		_sizeRatio(std::move(p_other._sizeRatio))
	{

	}

	Widget& Widget::operator=(Widget&& p_other)
	{
		if (this != &p_other)
		{
			_isOperationnal = std::move(p_other._isOperationnal);
			_activationContract = std::move(p_other._activationContract);
			_deactivationContract = std::move(p_other._deactivationContract);
			_timeConsomptionMetrics = std::move(p_other._timeConsomptionMetrics);
			_name = std::move(p_other._name);
			_geometryEdited = std::move(p_other._geometryEdited);
			_depth = std::move(p_other._depth);
			_viewport = std::move(p_other._viewport);
			_anchorRatio = std::move(p_other._anchorRatio);
			_sizeRatio = std::move(p_other._sizeRatio);
		}
		return *this;
	}

	Widget::~Widget()
	{
    	transferChildrens(Application::instance()->_centralWidget());
	}

	void Widget::_computeResizeRatio()
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

	void Widget::_applyResizeOperation()
	{
		const spk::Vector2UInt& areaSize = (parent() == nullptr ? spk::Application::instance()->window().size() : parent()->viewport().area().size());
		
		_viewport.setGeometry(spk::Area(areaSize * _anchorRatio, areaSize * _sizeRatio));
		_geometryEdited = true;
	}

	void Widget::setAnchor(const spk::Vector2Int& p_anchor)
	{
		_viewport.setAnchor(p_anchor);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}

	void Widget::setSize(const spk::Vector2UInt& p_size)
	{
		_viewport.setSize(p_size);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}

	void Widget::setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size)
	{
		_viewport.setGeometry(spk::Area(p_anchor, p_size));

		_computeResizeRatio();
		
		_geometryEdited = true;
	}
	
	void Widget::setGeometry(const spk::Area& p_area)
	{
		_viewport.setGeometry(p_area);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}
	
	void Widget::place(const spk::Vector2Int& p_anchor)
	{
		_viewport.setAnchor(p_anchor);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}
	
	void Widget::move(const spk::Vector2Int& p_delta)
	{
		_viewport.setAnchor(_viewport.area().anchor() + p_delta);

		_computeResizeRatio();
		
		_geometryEdited = true;
	}

	void Widget::resize(const spk::Vector2UInt& p_size)
	{
		_viewport.setSize(p_size);
		
		_geometryEdited = true;
	}

	void Widget::setDepth(const float& p_depth)
	{
		float delta(p_depth - _depth);

		_depth = p_depth;
		
		for (auto child : childrens())
		{
			child->setDepth(child->depth() + delta);
		}
	}

	void Widget::rename(const std::wstring& p_name)
	{
		_name = p_name;
	}

	spk::Widget* Widget::searchWidget(const std::wstring& p_name) const
	{
		for (size_t i = 0; i < childrens().size(); i++)
		{
			if (childrens()[i]->name() == p_name)
				return (childrens()[i]);
		}
		for (size_t i = 0; i < childrens().size(); i++)
		{
			Widget* childrenResult = childrens()[i]->searchWidget(p_name);
			if (childrenResult != nullptr)
				return (childrenResult);
		}
		return (nullptr);
	}

	bool Widget::isPointed(const spk::Vector2Int& p_point)
	{
		return (_viewport.isInside(p_point));
	}
};