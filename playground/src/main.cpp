#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:
    spk::WidgetComponent::Box _box;
    spk::WidgetComponent::TextLabel _textLabel;

    void _onGeometryChange()
    {
        spk::Vector2Int center = size() / spk::Vector2UInt(2, 2);
        spk::Vector2UInt tmpSize = spk::Vector2UInt(10, 10);
        _box.area() = spk::Area(center - tmpSize / spk::Vector2UInt(2, 2), tmpSize);
        _box.depth() = depth();

        _textLabel.anchor() = center;
        _textLabel.depth() = depth() + 1;
    }
    void _onRender()
    {
        _box.render();
        _textLabel.render();
    }

    bool _onUpdate()
    {
        return (false);
    }

public:
    Test(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _textLabel(),
        _box()
    {
        _textLabel.defaultFont = std::make_shared<spk::Font>(L"Roboto-Regular.ttf");
        _textLabel.text() = L"Coucou ceci est un test";
        _textLabel.textSize() = 50;
        _textLabel.textColor() = spk::Color(255, 255, 255, 255);
        _textLabel.verticalAlignment() = spk::Font::VerticalAlignment::Top;
        _textLabel.outlineSize() = 2;
        _textLabel.outlineColor() = spk::Color(120, 120, 120, 255);

        _box.backgroundColor() = spk::Color(250, 60, 60, 255);
        _box.frontgroundColor() = spk::Color(250, 120, 120, 255);
        _box.borderSize() = 2;
    }

    ~Test()
    {

    }
};

int main()
{
    spk::Application app(L"Playground", 800);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
  
    std::shared_ptr<Test> test = app.addRootWidget<Test>(L"MainWidget");
    test->setGeometry(spk::Vector2Int(0, 0), app.size());
    test->activate();

    return (app.run());
}
