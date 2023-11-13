#include "playground.hpp"


class GrandGrandGrandChildrenWidget : public spk::Widget::Interface
{
private:
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        DEBUG_LINE();
        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
        DEBUG_LINE();
    }

    void _onRender()
    {
        _box.render();
    }

    bool _onUpdate()
    {
        return (false);
    }

public:
    GrandGrandGrandChildrenWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        DEBUG_LINE();
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(0, 0, 255, 255);
        _box.frontgroundColor() = spk::Color(0, 0, 80, 255);
        DEBUG_LINE();
    }

    ~GrandGrandGrandChildrenWidget()
    {

    }
};

class GrandGrandChildrenWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<GrandGrandGrandChildrenWidget> _grandGrandGrandChildrenWidget;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        DEBUG_LINE();
		_grandGrandGrandChildrenWidget->setGeometry(25, size() - spk::Vector2UInt(100, 100));
        
        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
        DEBUG_LINE();
    }

    void _onRender()
    {
        _box.render();
    }

    bool _onUpdate()
    {
        return (false);
    }

public:
    GrandGrandChildrenWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        DEBUG_LINE();
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(255, 255, 255, 255);
        _box.frontgroundColor() = spk::Color(50, 50, 50, 255);

		_grandGrandGrandChildrenWidget = addChildrenWidget<GrandGrandGrandChildrenWidget>(L"GrandGrandGrandChildrenWidget");
		_grandGrandGrandChildrenWidget->activate();
        DEBUG_LINE();
    }

    ~GrandGrandChildrenWidget()
    {

    }
};

class GrandChildrenWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<GrandGrandChildrenWidget> _grandGrandChildrenWidget;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        DEBUG_LINE();
        _grandGrandChildrenWidget->setGeometry(25, size() - spk::Vector2UInt(100, 100));

        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
        DEBUG_LINE();
    }
    void _onRender()
    {
        _box.render();
    }

    bool _onUpdate()
    {
		// if (spk::TimeMetrics::instance()->time() % 1000 == 0)
		// 	_grandGrandChildrenWidget->setGeometry(spk::Vector2Int(rand() % 500 - 250, rand() % 500 - 250), size() - spk::Vector2UInt(100, 100));
        return (false);
    }

public:
    GrandChildrenWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        DEBUG_LINE();
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(255, 0, 0, 255);
        _box.frontgroundColor() = spk::Color(50, 0, 0, 255);

        _grandGrandChildrenWidget = addChildrenWidget<GrandGrandChildrenWidget>(L"GrandChildrenWidget");
        _grandGrandChildrenWidget->activate();
        DEBUG_LINE();
    }

    ~GrandChildrenWidget()
    {

    }
};

class ChildrenWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<GrandChildrenWidget> _grandChildrenWidget;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        DEBUG_LINE();
        _grandChildrenWidget->setGeometry(25, size() - spk::Vector2UInt(100, 100));

        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
        DEBUG_LINE();
    }
    void _onRender()
    {
        _box.render();
    }

    bool _onUpdate()
    {
        return (false);
    }

public:
    ChildrenWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        DEBUG_LINE();
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(0, 255, 0, 255);
        _box.frontgroundColor() = spk::Color(0, 50, 0, 255);

        _grandChildrenWidget = addChildrenWidget<GrandChildrenWidget>(L"GrandChildrenWidget");
        _grandChildrenWidget->activate();
        DEBUG_LINE();
    }

    ~ChildrenWidget()
    {

    }
};

class MainWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<ChildrenWidget> _childrenWidget;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        DEBUG_LINE();
        _childrenWidget->setGeometry(25, size() - spk::Vector2UInt(100, 100));

        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
        DEBUG_LINE();
    }
    void _onRender()
    {
        _box.render();
    }

    bool _onUpdate()
    {
        return (false);
    }

public:
    MainWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        DEBUG_LINE();
        _box.backgroundColor() = spk::Color(0, 0, 255, 255);
        _box.frontgroundColor() = spk::Color(0, 0, 50, 255);
        _box.borderSize() = 5;

        _childrenWidget = addChildrenWidget<ChildrenWidget>(L"ChildrenWidget");
        _childrenWidget->activate();
        DEBUG_LINE();
    }

    ~MainWidget()
    {

    }
};

int main()
{
    spk::Application app(L"Playground", 800);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
  
    std::shared_ptr<MainWidget> mainWidget = app.addRootWidget<MainWidget>(L"MainWidget");
    mainWidget->setGeometry(0, app.size());
    mainWidget->activate();

    return (app.run());
}
