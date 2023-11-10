#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:
    spk::WidgetComponent::TextLabel _textLabel;

    void _onGeometryChange()
    {
        _textLabel.anchor() = size() / spk::Vector2UInt(2, 2);
        _textLabel.textSize() = 50;
        _textLabel.textColor() = spk::Color(255, 255, 255, 255);
        _textLabel.outlineSize() = 2;
        _textLabel.outlineColor() = spk::Color(120, 120, 120, 255);
    }
    void _onRender()
    {
        _textLabel.render();
    }

    bool _onUpdate()
    {
        return (false);
    }

public:
    Test(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _textLabel()
    {
        _textLabel.defaultFont = std::make_shared<spk::Font>(L"Roboto-Regular.ttf");
        _textLabel.text() = L"Coucou ceci est un test";
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
