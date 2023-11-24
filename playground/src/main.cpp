#include "playground.hpp"

class MainWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<spk::GameEngineManager> _gameEngineManager;

    std::shared_ptr<spk::GameEngine> _engine;

    void _onGeometryChange()
    {
        _gameEngineManager->setGeometry(0, size());
    }

    void _onRender()
    {

    }

    bool _onUpdate()
    {
        return (false);
    }

    std::shared_ptr<spk::GameObject> createPlayer(const spk::Vector3& p_position, const spk::Vector3& p_playerTarget)
    {
        std::shared_ptr<spk::GameObject> result = std::make_shared<spk::GameObject>(L"Player", p_position);
        result->transform()->lookAt(p_playerTarget);
        result->addComponent<spk::Camera>();
        auto fpsController = result->addComponent<spk::FirstPersonController>();
        fpsController->setMouseControl(spk::FirstPersonController::MouseControl::PressedLeft);

        return (result);
    }

    std::shared_ptr<spk::GameObject> createCube(const spk::Vector3& p_position)
    {
        std::shared_ptr<spk::GameObject> result = std::make_shared<spk::GameObject>(L"Cube", p_position);
        std::shared_ptr<spk::MeshRenderer> meshRenderer = result->addComponent<spk::MeshRenderer>();
        meshRenderer->setMesh(std::make_shared<spk::Cube>());

        return (result);
    }

public:
    MainWidget(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        _engine = std::make_shared<spk::GameEngine>();
        _engine->addGameObject(createPlayer(spk::Vector3(-2, 2, -2), spk::Vector3(0, 0, 0)));

        for (size_t i = 0; i < 10; i++)
        {
            for (size_t j = 0; j < 10; j++)
            {
                _engine->addGameObject(createCube(spk::Vector3(i * 1.5f, 0, j * 1.5f)));
            }
        }

        _gameEngineManager = addChildrenWidget<spk::GameEngineManager>(L"GameEngineManager");
        _gameEngineManager->setGameEngine(_engine);
        _gameEngineManager->activate();
    }

    ~MainWidget()
    {

    }
};


int main()
{
    spk::Application app(L"Playground", 900);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

    std::shared_ptr<MainWidget> mainWidget = app.addRootWidget<MainWidget>(L"MainWidget");
    mainWidget->setGeometry(0, app.size());
    mainWidget->activate();

    return (app.run());
}
