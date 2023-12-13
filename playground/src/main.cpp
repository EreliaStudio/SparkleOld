#include "playground.hpp"

struct Chunk : public spk::GameObject
{
public:
    static inline std::shared_ptr<spk::SpriteSheet> SpriteSheet = nullptr;
    static inline std::shared_ptr<spk::Material> Material = nullptr;

private:
    static const int Size = 16;
    static inline spk::Perlin2D _perlinGeneration = spk::Perlin2D(987654321);

    spk::Vector2Int _position;
    std::shared_ptr<spk::MeshRenderer> _renderer;
    std::shared_ptr<spk::Mesh> _mesh;

    float height[Size + 1][Size + 1];

    float _generateHeight(const int& p_x, const int& p_y)
    {
        float result = _perlinGeneration.sample(static_cast<float>(p_x + _position.x * Size + 150000), static_cast<float>(p_y + _position.y * Size + 150000)) * 10;
        return (result);
    }

    spk::Vector3 _generatePoint(const int& p_x, const int& p_y)
    {
        spk::Vector3 result = spk::Vector3(p_x, (p_x == -1 || p_y == -1 || p_x >= Size + 1 || p_y >= Size + 1 ? _generateHeight(p_x, p_y) : height[p_x][p_y]), p_y);
        if (result.y <= 0.0f)
            result.y = 0.0f;
        return (result);
    }

    void _bakePoints()
    {
        for (size_t y = 0; y <= Size; y++)
        {
            for (size_t x = 0; x <= Size; x++)
            {
                _mesh->points().push_back(_generatePoint(x, y));
            }
        }
    }

    void _bakeUVs()
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
    }

    void _bakeNormales()
    {
        for (int y = 0; y <= Size; y++)
        {
            for (int x = 0; x <= Size; x++)
            {
                spk::Vector3 points[5] = {
                    _generatePoint(x +  0, y +  0),
                    _generatePoint(x + -1, y +  0),
                    _generatePoint(x +  0, y + -1),
                    _generatePoint(x +  0, y +  1),
                    _generatePoint(x +  1, y +  0)
                };

                int trianglesIndexes[4][2] = {
                    {1, 3}, {3, 4}, {4, 2}, {2, 1},
                };

                spk::Vector3 normal = spk::Vector3();

                for (size_t i = 0; i < 4; i++)
                {
                    spk::Vector3 vectorAB = points[trianglesIndexes[i][0]] - points[0];
                    spk::Vector3 vectorAC = points[trianglesIndexes[i][1]] - points[0];

                    normal += vectorAB.cross(vectorAC).normalize();
                }
                normal /= 4;

                _mesh->normals().push_back(normal);
            }
        }
    }

    void _bakeRawData()
    {
        _bakePoints();
        _bakeUVs();
        _bakeNormales();		
    }

    size_t _computeSpriteID(const spk::Vector2Int& p_a, const spk::Vector2Int& p_b, const spk::Vector2Int& p_c)
    {
        float value = std::max(
                std::max(height[p_a.x][p_a.y], height[p_b.x][p_b.y]),
                height[p_c.x][p_c.y]
            );

        if (value < -5.0f)
            return (4);
        else if (value < 0.0f)
            return (3);
        else if (value < 15.0f)
            return (2);
        else if (value < 25.0f)
            return (1);
        else
            return (0);
    }

    void _bakeVertex()
    {
        size_t pointOffsets[6] = {Size + 1, Size + 2, 1, Size + 1, 1, 0};
        size_t uvsOffsets[6] = {0, 1, 2, 0, 2, 3};
        spk::Vector2Int positionOffset[2][3] = {
            {
                spk::Vector2Int(0, 1),
                spk::Vector2Int(1, 1),
                spk::Vector2Int(1, 0)
            },
            {
                spk::Vector2Int(1, 0),
                spk::Vector2Int(0, 1),
                spk::Vector2Int(0, 0)
            }
        };

        for (size_t y = 0; y < Size; y++)
        {
            for (size_t x = 0; x < Size; x++)
            {
                spk::Vector2Int position = spk::Vector2Int(x, y);
                size_t baseIndex = x + y * (Size + 1);

                for (size_t i = 0; i < 2; i++)
                {
                    size_t baseSprite = _computeSpriteID(position + positionOffset[i][0], position + positionOffset[i][1], position + positionOffset[i][2]);

                    for (size_t j = 0; j < 3; j++)
                    {
                        _mesh->addVertex(baseIndex + pointOffsets[i * 3 + j], baseSprite * 4 + uvsOffsets[i * 3 + j], baseIndex + pointOffsets[i * 3 + j]);
                    }
                }
            }
        }
    }

    void _bakeFaces()
    {
        for (size_t i = 0; i < _mesh->vertices().size(); i += 3)
        {
            _mesh->addFace(i + 0, i + 1, i + 2);
        }
        _mesh->setNeedUpdateFlag(true);
    }

    void _bake()
    {
        _bakeRawData();

        _bakeVertex();
        
        _bakeFaces();
    }

public:
    static void InitializePerlinGeneration()
    {
        _perlinGeneration.configureInterpolation(spk::Perlin2D::Interpolation::SmoothStep);
        _perlinGeneration.configureRange(-5, 5);
        _perlinGeneration.configureFrequency(60);
        _perlinGeneration.configureOctave(7);
        _perlinGeneration.configureLacunarity(2.0f);
        _perlinGeneration.configurePersistance(0.5f);
    }

    Chunk(const spk::Vector2Int& p_position) :
        spk::GameObject(L"Chunk [" + spk::to_wstring(p_position) + L"]", spk::Vector3(Size * p_position.x, 0, Size * p_position.y)),
        _renderer(addComponent<spk::MeshRenderer>()),
        _mesh(std::make_shared<spk::Mesh>()),
        _position(p_position)
    {
        _renderer->setTexture(SpriteSheet);
        _renderer->setMaterial(Material);

        for (size_t i = 0; i <= Size; i++)
        {
            for (size_t j = 0; j <= Size; j++)
            {
                height[i][j] = _generateHeight(i, j);
            }
        }

        _bake();
        _renderer->setMesh(_mesh);
    }

    const spk::Vector2Int& position() const
    {
        return (_position);
    }

    static spk::Vector2Int ConvertWorldToChunkPosition(const spk::Vector3& p_worldPosition)
    {
        return (spk::Vector2Int(
            static_cast<int>(p_worldPosition.x) / Size - ((p_worldPosition.x < 0 && static_cast<int>(p_worldPosition.x) % Size != 0) ? 1 : 0), 
            static_cast<int>(p_worldPosition.z) / Size - ((p_worldPosition.z < 0 && static_cast<int>(p_worldPosition.z) % Size != 0) ? 1 : 0)
        ));
    }
};

struct World
{
private:
    std::map<spk::Vector2Int, std::shared_ptr<Chunk>> _chunks;

public:
    World()
    {
        Chunk::InitializePerlinGeneration();
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
    spk::Vector2Int _playerVisionSize = spk::Vector2Int(15, 15);

    std::shared_ptr<spk::GameEngineManager> _gameEngineManager;
    std::vector<std::shared_ptr<Chunk>> _activeChunks;

    std::shared_ptr<spk::GameEngine> _engine;
    std::shared_ptr<spk::GameObject> _player;
    std::shared_ptr<spk::GameObject> _lights;
    std::shared_ptr<spk::DirectionalLight> _directionLight;

    void _onGeometryChange()
    {
        _gameEngineManager->setGeometry(0, size());
    }

    std::shared_ptr<Chunk> _generateChunk(const spk::Vector2Int& p_chunkPosition)
    {
        std::shared_ptr<Chunk> newChunk = _world.chunk(p_chunkPosition);

        _engine->addGameObject(newChunk);
        newChunk->activate();

        return (newChunk);
    }

    void _onRender()
    {
        
    }

    void _deactivateOutOffBoundChunk(const spk::Vector2Int& p_startPosition, const spk::Vector2Int& p_endPosition)
    {
        _activeChunks.erase(
            std::remove_if(
                _activeChunks.begin(),
                _activeChunks.end(),
                [&p_startPosition, &p_endPosition](const auto& chunkPtr) {
                    if (spk::Vector2Int::isInsideRectangle(chunkPtr->position(), p_startPosition, p_endPosition) == false) {
                        chunkPtr->deactivate();
                        return (true);
                    }
                    return (false);
                }
            ),
            _activeChunks.end()
        );
    }

    bool _onUpdate()
    {
        static spk::Vector2Int lastPlayerChunkPosition = Chunk::ConvertWorldToChunkPosition(_player->transform()->translation()) - spk::Vector2Int(1, 1);
        spk::Vector2Int playerChunkPosition = Chunk::ConvertWorldToChunkPosition(_player->transform()->translation());
        if (playerChunkPosition != lastPlayerChunkPosition)
        {
            spk::Vector2Int startPosition = playerChunkPosition - _playerVisionSize / spk::Vector2(2, 2);
            spk::Vector2Int endPosition = playerChunkPosition + _playerVisionSize / spk::Vector2(2, 2);

            _deactivateOutOffBoundChunk(startPosition, endPosition);
    
            for (int x = startPosition.x; x <= endPosition.x; x++)
            {
                for (int y = startPosition.y; y <= endPosition.y; y++)
                {
                    spk::Vector2Int chunkPosition = spk::Vector2Int(x, y);

                    if (_world.contains(chunkPosition) == false)
                    {
                        std::shared_ptr<Chunk> newChunk = _generateChunk(chunkPosition);
                        _activeChunks.push_back(newChunk);
                    }
                    else
                    {
                        std::shared_ptr<Chunk> chunk = _world.chunk(chunkPosition);
                        if (chunk->isActive() == false)
                        {
                            chunk->activate();
                            _activeChunks.push_back(chunk);
                        }
                    }
                }
            }
            lastPlayerChunkPosition = playerChunkPosition;
        }
        
        static long long nextInput = spk::TimeMetrics::instance()->time();

        if (spk::Application::instance()->keyboard().inputStatus(spk::Keyboard::A) == spk::InputState::Down && nextInput <= spk::TimeMetrics::instance()->time())
        {
            _directionLight->setDirection(_directionLight->direction().rotate(spk::Vector3(0, 5, 0)));
            nextInput = spk::TimeMetrics::instance()->time() + 100;
        }
        else if (spk::Application::instance()->keyboard().inputStatus(spk::Keyboard::E) == spk::InputState::Down && nextInput <= spk::TimeMetrics::instance()->time())
        {
            _directionLight->setDirection(_directionLight->direction().rotate(spk::Vector3(0, -5, 0)));
            nextInput = spk::TimeMetrics::instance()->time() + 100;
        }

        return (false);
    }

    std::shared_ptr<spk::GameObject> createPlayer(const spk::Vector3& p_position, const spk::Vector3& p_playerTarget)
    {
        std::shared_ptr<spk::GameObject> result = std::make_shared<spk::GameObject>(L"Player", p_position);

        result->transform()->lookAt(p_playerTarget);
        auto camera = result->addComponent<spk::Camera>();
        auto fpsController = result->addComponent<spk::FirstPersonController>();
        fpsController->setMouseControl(spk::FirstPersonController::MouseControl::PressedLeft);
        fpsController->setMovementSpeed(15.0f);

        return (result);
    }

    std::shared_ptr<spk::GameObject> createLights(const spk::Vector3& p_direction)
    {
        std::shared_ptr<spk::GameObject> result = std::make_shared<spk::GameObject>(L"Lights", spk::Vector3(0, 0, 0));

        auto tmp = result->addComponent<spk::DirectionalLight>();
        tmp->setDirection(spk::Vector3(-1.0f, -1.0f, 0));
        tmp->setIntensity(0.25f);
        tmp->setColor(spk::Color(255, 255, 255));

        return (result);
    }

public:
    MainWidget(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        Chunk::SpriteSheet = std::make_shared<spk::SpriteSheet>(L"worldChunkTexture.png", spk::Vector2UInt(5, 1));
        Chunk::Material = std::make_shared<spk::Material>(0.25f, 32.0f);

        _engine = std::make_shared<spk::GameEngine>();

        _player = createPlayer(spk::Vector3(2, 2, 2), spk::Vector3(0, 0, 0));
        _engine->addGameObject(_player);

        _lights = createLights(spk::Vector3(0, -1, 0));
        _directionLight = _lights->getComponent<spk::DirectionalLight>();
        _engine->addGameObject(_lights);

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

    std::shared_ptr<MainWidget> mainWidget = app.addRootWidget<MainWidget>(L"MainWidget");
    mainWidget->setGeometry(0, app.size());
    mainWidget->activate();

    return (app.run());
}
