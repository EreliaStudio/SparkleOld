#include "playground.hpp"


class CyanWidget : public spk::Widget::Interface
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
    CyanWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(0, 255, 255, 255);
        _box.frontgroundColor() = spk::Color(0, 80, 80, 255);
    }

    ~CyanWidget()
    {

    }
};

class WhiteWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<CyanWidget> _cyanWidget;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
		_cyanWidget->setGeometry(-25, size() + spk::Vector2UInt(50, 50));

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
    WhiteWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(255, 255, 255, 255);
        _box.frontgroundColor() = spk::Color(50, 50, 50, 255);

		_cyanWidget = addChildrenWidget<CyanWidget>(L"CyanWidget");
		_cyanWidget->activate();
    }

    ~WhiteWidget()
    {

    }
};

class RedWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<WhiteWidget> _whiteWidget;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        _whiteWidget->setGeometry(-243, size() - spk::Vector2UInt(100, 100));

        _box.area() = spk::Area(anchor(), size());
        _box.depth() = depth();
    }
    void _onRender()
    {
        _box.render();
    }

    bool _onUpdate()
    {
        if (spk::TimeMetrics::instance()->time() % 250 == 0)
        {
            spk::Vector2Int newAnchor = spk::Vector2Int(rand() % 500 - 250, rand() % 500 - 250);
            _whiteWidget->setGeometry(newAnchor, size() - spk::Vector2UInt(100, 100));
        }
        return (false);
    }

public:
    RedWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(255, 0, 0, 255);
        _box.frontgroundColor() = spk::Color(50, 0, 0, 255);

        _whiteWidget = addChildrenWidget<WhiteWidget>(L"WhiteWidget");
        _whiteWidget->activate();
    }

    ~RedWidget()
    {

    }
};

class GreenWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<RedWidget> _redWidget;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        _redWidget->setGeometry(25, size() - spk::Vector2UInt(100, 100));

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
    GreenWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.borderSize() = 5;
        _box.backgroundColor() = spk::Color(0, 255, 0, 255);
        _box.frontgroundColor() = spk::Color(0, 50, 0, 255);

        _redWidget = addChildrenWidget<RedWidget>(L"RedWidget");
        _redWidget->activate();
    }

    ~GreenWidget()
    {

    }
};

class BlueWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<GreenWidget> _greenWidget;
    spk::WidgetComponent::Box _box;

    void _onGeometryChange()
    {
        _greenWidget->setGeometry(25, size() - spk::Vector2UInt(100, 100));

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
    BlueWidget(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _box()
    {
        _box.backgroundColor() = spk::Color(0, 0, 255, 255);
        _box.frontgroundColor() = spk::Color(0, 0, 50, 255);
        _box.borderSize() = 5;

        _greenWidget = addChildrenWidget<GreenWidget>(L"GreenWidget");
        _greenWidget->activate();
    }

    ~BlueWidget()
    {

    }
};

int main()
{
    spk::Application app(L"Playground", 800);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
  
    std::shared_ptr<BlueWidget> mainWidget = app.addRootWidget<BlueWidget>(L"BlueWidget");
    mainWidget->setGeometry(0, app.size());
    mainWidget->activate();

    return (app.run());
}
