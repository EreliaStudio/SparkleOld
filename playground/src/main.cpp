#include "playground.hpp"

class DebugScreenWidget : public spk::Widget::Interface
{
private:
    spk::RandomGenerator<int> _generator;
    std::shared_ptr<spk::Font> _font;
    std::vector<std::shared_ptr<spk::WidgetComponent::TextLabel>> _lines;

    void _onGeometryChange()
    {
        float height = static_cast<float>(size().y / _lines.size());
        float space = height * 0.1f;
        float textSize = height * 0.8f;

        for (size_t i = 0; i < _lines.size(); i++)
        {
            _lines[i]->anchor() = (spk::Vector2Int(space, space + height * i));
            _lines[i]->textSize() = static_cast<size_t>(textSize);
        }
    }

    void _onRender()
    {
        for (size_t i = 0; i < _lines.size(); i++)
        {
            _lines[i]->render();
        }
    }

    bool _onUpdate()
    {
        if (spk::TimeMetrics::instance()->time() % 1000 == 0)
        {
            spk::Profiler::instance()->startChronometer(L"DebugScreenWidget::_onUpdate");
            _lines[0]->text() = L"FPS : [" + std::to_wstring(spk::Profiler::instance()->FPS()) + L"]";
            _lines[1]->text() = L"UPS : [" + std::to_wstring(spk::Profiler::instance()->UPS()) + L"]";
            spk::Profiler::instance()->stopChronometer(L"DebugScreenWidget::_onUpdate");
        }
        return (false);
    }

public:
    DebugScreenWidget(std::shared_ptr<spk::GameObject> p_playerObject, const std::wstring& p_name) : 
        spk::Widget::Interface(p_name),
        _font(std::make_shared<spk::Font>(L"Roboto-Regular.ttf"))
    {
        spk::Profiler::instance()->createChronometer(L"DebugScreenWidget::_onUpdate");
        _generator.configureRange(0, 50);

        _lines.resize(25);
        for (size_t i = 0; i < _lines.size(); i++)
        {
            _lines[i] = std::make_shared<spk::WidgetComponent::TextLabel>();
            _lines[i]->font() = _font;
            _lines[i]->verticalAlignment() = spk::Font::VerticalAlignment::Top;
            _lines[i]->horizontalAlignment() = spk::Font::HorizontalAlignment::Left;
            _lines[i]->textColor() = spk::Color(225, 225, 225);
            _lines[i]->outlineColor() = spk::Color(50, 50, 50);
            _lines[i]->outlineSize() = 0;
            _lines[i]->outlineType() = spk::Font::OutlineType::Standard;
        }
    }

    ~DebugScreenWidget()
    {

    }
};

class GameEngineManager : public spk::Widget::Interface
{
private:
    std::shared_ptr<DebugScreenWidget> _debugScreenWidget;

    void _onGeometryChange()
    {
        if (spk::Camera::mainCamera() != nullptr)
		spk::Camera::mainCamera()->setPerspectiveParameters(90.0f, static_cast<float>(size().x) / static_cast<float>(size().y), 0.1f, 100.0f);

        _debugScreenWidget->setGeometry(0, size());
    }
    
    void _onRender()
    {

    }
    
    bool _onUpdate()
    {
        return (false);
    }

public:
    GameEngineManager(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        spk::Profiler::instance()->startChronometer(L"GameEngineManagerConstructor");
        _debugScreenWidget = addChildrenWidget<DebugScreenWidget>(nullptr, L"DebugScreen");
        _debugScreenWidget->activate();
        spk::Profiler::instance()->stopChronometer(L"GameEngineManagerConstructor");
    }

    ~GameEngineManager()
    {
        
    }
};


int main()
{
    spk::Application app(L"Playground", 900);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
  
    std::shared_ptr<GameEngineManager> gameEngineManager = app.addRootWidget<GameEngineManager>(L"GameEngineManager");
    gameEngineManager->setGeometry(0, app.size());
    gameEngineManager->activate();

    return (app.run());
}
