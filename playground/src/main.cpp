#include "playground.hpp"

class DebugScreenWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<spk::GameObject> _playerObject;
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
            _lines[0]->text() = L"FPS : [" + std::to_wstring(spk::Profiler::instance()->FPS()) + L"]";
            _lines[1]->text() = L"UPS : [" + std::to_wstring(spk::Profiler::instance()->UPS()) + L"]";
        }
        return (false);
    }

public:
    DebugScreenWidget(std::shared_ptr<spk::GameObject> p_playerObject, const std::wstring& p_name) : 
        spk::Widget::Interface(p_name),
        _playerObject(p_playerObject),
        _font(std::make_shared<spk::Font>(L"Roboto-Regular.ttf"))
    {
        _playerObject->transform()->subscribeOnTranslation([&](){
            _lines[3]->text() = L"Position : " + _playerObject->transform()->translation().to_wstring();
        });
        _playerObject->transform()->subscribeOnScaling([&](){
            _lines[4]->text() = L"Scale : " + _playerObject->transform()->scale().to_wstring();
        });
        _playerObject->transform()->subscribeOnRotation([&](){
            _lines[5]->text() = L"Rotation : " + _playerObject->transform()->rotation().to_wstring();
        });
        
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
        _lines[3]->text() = L"Position : " + _playerObject->transform()->translation().to_wstring();
        _lines[4]->text() = L"Scale : " + _playerObject->transform()->scale().to_wstring();
        _lines[5]->text() = L"Rotation : " + _playerObject->transform()->rotation().to_wstring();
    }

    ~DebugScreenWidget()
    {

    }
};

class PlayerManager : public spk::Widget::Interface
{
private:
    spk::Vector3 _requestedDeltaPosition;
    std::shared_ptr<spk::GameObject> _playerObject;
    std::vector<std::shared_ptr<spk::Input>> _inputs;
    bool _cameraRotationEnable = true;

    void _onGeometryChange()
    {

    }

    void _onRender()
    {

    }

    bool _onUpdate()
    {
        for (size_t i = 0; i < _inputs.size(); i++)
        {
            _inputs[i]->update();
        }
        if (_requestedDeltaPosition != spk::Vector3(0, 0, 0))
        {
            _playerObject->transform()->move(_requestedDeltaPosition.normalize() * 0.2f);
            _requestedDeltaPosition = spk::Vector3(0, 0, 0);
        }
        return (false);
    }

public:
    PlayerManager(std::shared_ptr<spk::GameObject> p_playerObject, const std::wstring& p_name) : 
        spk::Widget::Interface(p_name),
        _playerObject(p_playerObject),
        _inputs{
            std::make_shared<spk::KeyInput>(spk::Keyboard::Z, spk::InputState::Down, 10, [&](){
                    _requestedDeltaPosition += (_playerObject->transform()->forward());
                }),
            std::make_shared<spk::KeyInput>(spk::Keyboard::Q, spk::InputState::Down, 10, [&](){
                    _requestedDeltaPosition += (_playerObject->transform()->right());
                }),
            std::make_shared<spk::KeyInput>(spk::Keyboard::S, spk::InputState::Down, 10, [&](){
                    _requestedDeltaPosition += (-_playerObject->transform()->forward());
                }),
            std::make_shared<spk::KeyInput>(spk::Keyboard::D, spk::InputState::Down, 10, [&](){
                    _requestedDeltaPosition += (-_playerObject->transform()->right());
                }),
            std::make_shared<spk::KeyInput>(spk::Keyboard::Space, spk::InputState::Down, 10, [&](){
                    _requestedDeltaPosition += (spk::Vector3(0, 1, 0));
                }),
            std::make_shared<spk::KeyInput>(spk::Keyboard::LeftShift, spk::InputState::Down, 10, [&](){
                    _requestedDeltaPosition += (spk::Vector3(0, -1, 0));
                }),
            std::make_shared<spk::KeyInput>(spk::Keyboard::Escape, spk::InputState::Pressed, 0, [&](){
                _cameraRotationEnable = !_cameraRotationEnable;
                if (_cameraRotationEnable == true)
                    spk::Mouse::instance()->place(spk::Vector2Int(spk::Window::instance()->size() / spk::Vector2UInt(2, 2)));
            }),
            std::make_shared<spk::MouseMovementInput>(1, [&](){
                if (_cameraRotationEnable == true)
                {
                    _playerObject->transform()->rotate(spk::Vector3(-spk::Mouse::instance()->deltaPosition().y, spk::Mouse::instance()->deltaPosition().x, 0));
                    spk::Mouse::instance()->place(spk::Vector2Int(spk::Window::instance()->size() / spk::Vector2UInt(2, 2)));
                }
            })
            }
    {
        spk::Mouse::instance()->place(spk::Vector2Int(spk::Window::instance()->size() / spk::Vector2UInt(2, 2)));
    }

    ~PlayerManager()
    {

    }
};

class GameEngineManager : public spk::Widget::Interface
{
private:
    std::shared_ptr<DebugScreenWidget> _debugScreenWidget;

    std::shared_ptr<PlayerManager> _playerManager;

    std::shared_ptr<spk::GameEngine> _gameEngine = nullptr;
    std::shared_ptr<spk::GameObject> _playerObject = nullptr;
    std::shared_ptr<spk::Camera> _cameraComponent = nullptr;

    void _onGeometryChange()
    {
		spk::Camera::mainCamera()->setPerspectiveParameters(90.0f, static_cast<float>(size().x) / static_cast<float>(size().y), 0.1f, 100.0f);

        _debugScreenWidget->setGeometry(0, size());
    }
    
    void _onRender()
    {
        if (_gameEngine != nullptr)
            _gameEngine->render();
    }
    
    bool _onUpdate()
    {
        if (_gameEngine != nullptr)
            _gameEngine->update();

        return (false);
    }
        
    void loadPlayer()
    {
        _playerObject = std::make_shared<spk::GameObject>(L"Player");

        _cameraComponent = _playerObject->addComponent<spk::Camera>(spk::Camera::Type::Perspective);
        _playerObject->transform()->place(spk::Vector3(-4, -5, -4));
        _playerObject->transform()->lookAt(spk::Vector3(2.5f, 0, 2.5f));
        _playerObject->activate();

        _gameEngine->subscribe(_playerObject);
    }

    void loadCubes()
    {
        std::shared_ptr<spk::GameObject> cubeObject = std::make_shared<spk::GameObject>(L"Cube Zero");

        _gameEngine->subscribe(cubeObject);

        for (size_t i = 0; i < 5; i++)
        {
            for (size_t j = 0; j < 5; j++)
            {
                cubeObject = std::make_shared<spk::GameObject>(L"Cube x[" + std::to_wstring(i) + L"]");

                auto tmp = cubeObject->addComponent<spk::MeshRenderer>();
                tmp->setMesh(std::make_shared<spk::Cube>());
                cubeObject->transform()->setScale(spk::Vector3(1, 0.5 + 0.4 * (i + 1 + j), 1));
                cubeObject->transform()->setRotation(spk::Vector3(0, 1 * (i + 1), 0));
                cubeObject->transform()->place(spk::Vector3(i * 2, 0, j * 2));
                cubeObject->activate();

                _gameEngine->subscribe(cubeObject);
            }
        }

        for (size_t i = 0; i < 5; i++)
        {
            for (size_t j = 0; j < 5; j++)
            {
                cubeObject = std::make_shared<spk::GameObject>(L"Cube x[" + std::to_wstring(i) + L"]");

                auto tmp = cubeObject->addComponent<spk::MeshRenderer>();
                tmp->setMesh(std::make_shared<spk::Cube>());
                cubeObject->transform()->setScale(spk::Vector3(1, 0.5 + 0.4 * (i + 1 + j), 1));
                cubeObject->transform()->setRotation(spk::Vector3(0, 1 * (i + 1), 0));
                cubeObject->transform()->place(spk::Vector3(i * 2, 10, j * 2));
                cubeObject->activate();

                _gameEngine->subscribe(cubeObject);
            }
        }
    }

public:
    GameEngineManager(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        _gameEngine = std::make_shared<spk::GameEngine>();
        loadPlayer();
        loadCubes();

        _playerManager = addChildrenWidget<PlayerManager>(_playerObject, L"PlayerManager");
        _playerManager->activate();

        _debugScreenWidget = addChildrenWidget<DebugScreenWidget>(_playerObject, L"DebugScreen");
        _debugScreenWidget->activate();
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
