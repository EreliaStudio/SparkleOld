#include "playground.hpp"


class GrandChildrenWidget : public spk::Widget::Interface
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
    GrandChildrenWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.backgroundColor() = spk::Color(255, 0, 0, 255);
        _box.frontgroundColor() = spk::Color(180, 0, 0, 255);
        _box.borderSize() = 5;
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
        _grandChildrenWidget->setGeometry(spk::Vector2Int(50, 50), size() - spk::Vector2UInt(100, 100));
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
    ChildrenWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(0, 255, 0, 255);
        _box.frontgroundColor() = spk::Color(0, 180, 0, 255);

        _grandChildrenWidget = addChildrenWidget<GrandChildrenWidget>(L"GrandChildrenWidget");
        _grandChildrenWidget->activate();
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
        _childrenWidget->setGeometry(spk::Vector2Int(50, 50), size() - spk::Vector2UInt(100, 100));

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
    MainWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.backgroundColor() = spk::Color(0, 0, 255, 255);
        _box.frontgroundColor() = spk::Color(0, 0, 180, 255);
        _box.borderSize() = 5;

        _childrenWidget = addChildrenWidget<ChildrenWidget>(L"ChildrenWidget");
        _childrenWidget->activate();
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
    mainWidget->setGeometry(spk::Vector2Int(0, 0), app.size());
    mainWidget->activate();

    return (app.run());
}
