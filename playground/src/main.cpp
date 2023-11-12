#include "playground.hpp"

class WidgetC : public spk::Widget::Interface
{
private:
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
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
    WidgetC(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(255, 0, 0, 255);
        _box.frontgroundColor() = spk::Color(180, 0, 0, 255);
    }

    ~WidgetC()
    {

    }
};

class WidgetB : public spk::Widget::Interface
{
private:
    std::shared_ptr<WidgetC> widgetC;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        widgetC->setGeometry(spk::Vector2Int(100, 100), size() - spk::Vector2UInt(200, 200));

        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
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
    WidgetB(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        widgetC = addChildrenWidget<WidgetC>(L"WidgetC");
        widgetC->activate();

        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(0, 255, 0, 255);
        _box.frontgroundColor() = spk::Color(0, 180, 0, 255);
    }

    ~WidgetB()
    {

    }
};

class WidgetA : public spk::Widget::Interface
{
private:
    std::shared_ptr<WidgetB> widgetB;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        widgetB->setGeometry(spk::Vector2Int(100, 100), size() - spk::Vector2UInt(100, 100));

        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
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
    WidgetA(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.backgroundColor() = spk::Color(0, 0, 255, 255);
        _box.frontgroundColor() = spk::Color(0, 0, 180, 255);
        _box.borderSize() = 5;

        widgetB = addChildrenWidget<WidgetB>(L"WidgetB");
        widgetB->activate();
    }

    ~WidgetA()
    {

    }
};

int main()
{
    spk::Application app(L"Playground", 800);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
  
    std::shared_ptr<WidgetA> mainWidget = app.addRootWidget<WidgetA>(L"WidgetA");
    mainWidget->setGeometry(spk::Vector2Int(0, 0), app.size());
    mainWidget->activate();

    return (app.run());
}
