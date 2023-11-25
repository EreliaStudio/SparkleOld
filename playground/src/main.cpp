#include "playground.hpp"

struct Chunk : public spk::GameObject
{
public:
    static inline std::shared_ptr<spk::SpriteSheet> SpriteSheet = nullptr;
private:
    static const size_t Size = 16;
    static inline spk::Perlin2D _perlinGeneration = spk::Perlin2D(123456);

    std::shared_ptr<spk::MeshRenderer> _renderer;
    std::shared_ptr<spk::Mesh> _mesh;

    float height[Size + 1][Size + 1];

    void _bakeRawData()
    {
        spk::Vector2 uvsValues[4] = {
            spk::Vector2(0, 0),
            spk::Vector2(1, 0),
            spk::Vector2(1, 1),
            spk::Vector2(0, 1)
        };

        for (const auto& sprite : SpriteSheet->sprites())
        {
            for (size_t j = 0; j < 4; j++)
            {
                _mesh->uvs().push_back(uvsValues[j] * SpriteSheet->unit() + sprite);
            }
        }

        for (size_t y = 0; y <= Size; y++)
        {
            for (size_t x = 0; x <= Size; x++)
            {
                _mesh->points().push_back(spk::Vector3(x, height[x][y] * 10 + 5, y));
                _mesh->normals().push_back(spk::Vector3(0, 1, 0));
            }
        }
    }

    size_t _computeSpriteID(const int& p_x, const int& p_y)
    {
        float value = (height[p_x][p_y] + height[p_x + 1][p_y] + height[p_x][p_y + 1] + height[p_x + 1][p_y + 1]) / 4;
        
        if (value < -0.2)
            return (4);
        else if (value < 0.0f)
            return (3);
        else if (value < 0.2f)
            return (2);
        else if (value < 0.3f)
            return (1);
        else
            return (0);
    }

    void _bakeVertex()
    {
        size_t pointOffsets[4] = {Size + 1, Size + 2, 1, 0};
        size_t uvsOffsets[4] = {0, 1, 2, 3};
        size_t normalOffsets[4] = {Size + 1, Size + 2, 1, 0};

        for (size_t y = 0; y < Size; y++)
        {
            for (size_t x = 0; x < Size; x++)
            {
                size_t baseIndex = x + y * (Size + 1);
                size_t baseSprite = _computeSpriteID(x, y);

                for (size_t i = 0; i < 4; i++)
                {
                    _mesh->addVertex(baseIndex + pointOffsets[i], baseSprite + uvsOffsets[i], baseIndex + normalOffsets[i]);
                }
            }
        }
    }

    void _bakeFaces()
    {
        for (size_t i = 0; i < _mesh->vertices().size(); i += 4)
        {
            _mesh->addFace(i + 0, i + 1, i + 2, i + 3);
        }
    }

    void _bake()
    {
        _bakeRawData();

        _bakeVertex();
        
        _bakeFaces();
    }

public:
    Chunk(const spk::Vector2Int& p_position) :
        spk::GameObject(L"Chunk [" + spk::to_wstring(p_position) + L"]", spk::Vector3(Size * p_position.x, 0, Size * p_position.y)),
        _renderer(addComponent<spk::MeshRenderer>()),
        _mesh(std::make_shared<spk::Mesh>())
    {
        if (SpriteSheet == nullptr)
            SpriteSheet = std::make_shared<spk::SpriteSheet>(L"worldChunkTexture.png", spk::Vector2UInt(5, 1));

        _renderer->setTexture(SpriteSheet);
        _perlinGeneration.configureRange(-1, 1);
        _perlinGeneration.configureFrequency(10);
        _perlinGeneration.configureOctave(3);
        _perlinGeneration.configureLacunarity(2.0f);
        _perlinGeneration.configurePersistance(0.5f);

        for (size_t i = 0; i <= Size; i++)
        {
            for (size_t j = 0; j <= Size; j++)
            {
                height[i][j] = _perlinGeneration.sample(static_cast<float>(p_position.x * Size + i), static_cast<float>(p_position.y * Size + j));
            }
        }

        _bake();
        _renderer->setMesh(_mesh);
    }
};

struct World
{
private:
    std::map<spk::Vector2Int, std::shared_ptr<Chunk>> _chunks;

public:
    World()
    {
    }
    
    bool contains(const spk::Vector2Int& p_chunkPosition) const
    {
        if (_chunks.contains(p_chunkPosition) == false)
            return (false);
        return (true);
    }

    std::shared_ptr<Chunk> chunk(const spk::Vector2Int& p_chunkPosition)
    {
        if (contains(p_chunkPosition) == true)
            return (_chunks[p_chunkPosition]);

        std::shared_ptr<Chunk> result = std::make_shared<Chunk>(p_chunkPosition);
        result->activate();
    
        _chunks[p_chunkPosition] = result;

        return (result);
    }
    
    std::shared_ptr<Chunk> chunk(const spk::Vector2Int& p_chunkPosition) const
    {
        if (contains(p_chunkPosition) == false)
            return (nullptr);
        return (_chunks.at(p_chunkPosition));
    }
};

class MainWidget : public spk::Widget::Interface
{
private:
    World _world;

    std::shared_ptr<spk::GameEngineManager> _gameEngineManager;

    std::shared_ptr<spk::GameEngine> _engine;
    std::shared_ptr<spk::GameObject> _player;

    void _onGeometryChange()
    {
        _gameEngineManager->setGeometry(0, size());
    }

    void _generateChunk(const spk::Vector2Int& p_chunkPosition)
    {
        std::shared_ptr<Chunk> newChunk = _world.chunk(p_chunkPosition);

        _engine->addGameObject(newChunk);
    }

    void _onRender()
    {
        if (_world.contains(spk::Vector2Int(0, 0)) == false)
        {
            _generateChunk(spk::Vector2Int(0, 0));
        }
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

public:
    MainWidget(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        _engine = std::make_shared<spk::GameEngine>();
        _player = createPlayer(spk::Vector3(2, 2, 2), spk::Vector3(0, 0, 0));
        _engine->addGameObject(_player);

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
