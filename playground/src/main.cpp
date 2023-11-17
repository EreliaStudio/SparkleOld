#include "playground.hpp"

class GameEngineManager : public spk::Widget::Interface
{
private:
    std::shared_ptr<spk::GameEngine> _gameEngine = nullptr;
    std::shared_ptr<spk::GameObject> _playerObject = nullptr;
    std::shared_ptr<spk::Camera> _cameraComponent = nullptr;

    void _onGeometryChange()
    {
		spk::Camera::mainCamera()->setPerspectiveParameters(90.0f, static_cast<float>(size().x) / static_cast<float>(size().y), 0.1f, 100.0f);
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
        _playerObject->transform()->place(spk::Vector3(-2, 2, -2));
        _playerObject->transform()->lookAt(spk::Vector3(0, 0, 0));
        _playerObject->activate();

        _gameEngine->subscribe(_playerObject);
    }

    void loadCubes()
    {
        std::shared_ptr<spk::GameObject> cubeObject = std::make_shared<spk::GameObject>(L"Cube Zero");

        auto tmp = cubeObject->addComponent<spk::MeshRenderer>();
        tmp->setMesh(std::make_shared<spk::Cube>());
        cubeObject->transform()->place(spk::Vector3(0, 0, 0));
        cubeObject->transform()->setScale(spk::Vector3(1, 0.5, 1));
        cubeObject->transform()->setRotation(spk::Vector3(0, 45, 0));
        cubeObject->activate();

        _gameEngine->subscribe(cubeObject);

        for (size_t i = 0; i < 4; i++)
        {
            cubeObject = std::make_shared<spk::GameObject>(L"Cube x[" + std::to_wstring(i) + L"]");

            auto tmp = cubeObject->addComponent<spk::MeshRenderer>();
            tmp->setMesh(std::make_shared<spk::Cube>());
            cubeObject->transform()->setScale(spk::Vector3(1, 0.5 + 0.4 * (i + 1), 1));
            cubeObject->transform()->setRotation(spk::Vector3(1, 10 * (i + 1), 1));
            cubeObject->transform()->place(spk::Vector3(2 + i * 2, 0, 0));
            cubeObject->activate();

            _gameEngine->subscribe(cubeObject);
            
            cubeObject = std::make_shared<spk::GameObject>(L"Cube z[" + std::to_wstring(i) + L"]");

            tmp = cubeObject->addComponent<spk::MeshRenderer>();
            tmp->setMesh(std::make_shared<spk::Cube>());
            cubeObject->transform()->setScale(spk::Vector3(1, 0.5 + 0.4 * (i + 1), 1));
            cubeObject->transform()->place(spk::Vector3(0, 0, 2 + i * 2));
            cubeObject->activate();

            _gameEngine->subscribe(cubeObject);
        }
    }

public:
    GameEngineManager(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        _gameEngine = std::make_shared<spk::GameEngine>();
        loadPlayer();
        loadCubes();
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
