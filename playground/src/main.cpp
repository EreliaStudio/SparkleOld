#include "playground.hpp"

class WitnessWidget : public spk::Widget::Interface
{
private:
    spk::WidgetComponent::Box _box;
    spk::WidgetComponent::TextLabel _label;

    void _onGeometryChange()
    {
        _box.area() = spk::Area(anchor(), size());
        _label.anchor() = size() / spk::Vector2UInt(2, 2);

        _label.depth() = 10 + depth();
        _box.depth() = 5 + depth();
    }
    
    void _onRender()
    {
        _box.render();
        _label.render();
    }
    
    bool _onUpdate()
    {
        return (false);
    }

public:
    WitnessWidget(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        _label.defaultFont = std::make_shared<spk::Font>(L"Roboto-Regular.ttf");
        _label.text() = L"abcdefghijklmnopqrstuvwxyz";
        _label.textColor() = spk::Color(235, 235, 235, 255);
        _label.textSize() = 60;
        _label.verticalAlignment() = spk::Font::VerticalAlignment::Middle;
        _label.horizontalAlignment() = spk::Font::HorizontalAlignment::Middle;

        _label.outlineColor() = spk::Color(0, 0, 0, 255);
        _label.outlineSize() = 3;
        _label.outlineType() = spk::Font::OutlineType::Standard;

        _box.backgroundColor() = spk::Color(60, 60, 60, 255);
        _box.foregroundColor() = spk::Color(130, 130, 130, 255);
        _box.borderSize() = 10;
    }

    ~WitnessWidget()
    {

    }
};

class MeshRenderer
{
private:
    static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr;
    std::shared_ptr<spk::Pipeline::Object> _renderingObject;

    std::shared_ptr<spk::Mesh> _mesh;
    spk::Vector3 _position;

    void _initializeMesh()
    {
        _mesh = std::make_shared<spk::Mesh>();

        _mesh->points().push_back(spk::Vector3(-1, 1, 0));
        _mesh->points().push_back(spk::Vector3(1, 1, 0));
        _mesh->points().push_back(spk::Vector3(-1, -1, 0));
        _mesh->points().push_back(spk::Vector3(1, -1, 0));

        _mesh->uvs().push_back(spk::Vector2(0, 0));
        _mesh->uvs().push_back(spk::Vector2(1, 0));
        _mesh->uvs().push_back(spk::Vector2(0, 1));
        _mesh->uvs().push_back(spk::Vector2(1, 1));
        
        _mesh->normals().push_back(spk::Vector3(0, 0, 0));

        _mesh->addVertex(0, 0, 0);
        _mesh->addVertex(1, 1, 0);
        _mesh->addVertex(2, 2, 0);
        _mesh->addVertex(3, 3, 0);

        _mesh->addFace(0, 1, 2, 3);
    }

    void _initializeRenderData()
    {
		_renderingObject->storage().vertices().clear();
		_renderingObject->storage().indexes().clear();

        _renderingObject->storage().vertices() << _mesh->vertices() << std::endl;
        _renderingObject->storage().indexes() << _mesh->indexes() << std::endl;
    }

public:
    MeshRenderer()
    {
        if (_renderingPipeline == nullptr)
            _renderingPipeline = std::make_shared<spk::Pipeline>(spk::ShaderModule(L"testShader.vert"), spk::ShaderModule(L"testShader.frag"));

        _renderingObject = _renderingPipeline->createObject();

        _initializeMesh();

        _initializeRenderData();
    }

    MeshRenderer(const spk::Vector3& p_position) :
        MeshRenderer()
    {
        place(p_position);
    }

    void render()
    {
        _renderingObject->render();
    }

    void place(const spk::Vector3& p_position)
    {
        _renderingObject->pushConstants(L"translation") = p_position;
        _position = p_position;
    }

    std::shared_ptr<spk::Mesh>& mesh()
    {
        return (_mesh);
    }

    spk::Vector3& position() 
    {
        return _position;
    }
};

class GameEngineRendererWidget : public spk::Widget::Interface
{
private:
    static inline std::shared_ptr<spk::Pipeline::UniformBlock> _cameraInformationBlock = nullptr;
    static inline bool _cameraInformationBlockNeedUpdate = false;

    std::shared_ptr<WitnessWidget> _witnessWidget;

    spk::Vector3 _position = spk::Vector3(0, 0, 0);
    spk::Vector3 _forward = spk::Vector3(0, 0, 1);
    spk::Vector3 _right = spk::Vector3(1, 0, 0);

    std::vector<MeshRenderer> _objects;

    void _onGeometryChange()
    {
        _witnessWidget->setGeometry(spk::Vector2Int(10, 10), spk::Vector2UInt(800, 150));

        if (_cameraInformationBlock == nullptr)
            _cameraInformationBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(spk::Pipeline::uniform(spk::Pipeline::Uniform::Key(0, 0)));
            
        _cameraInformationBlock->field(L"depth") = 1;
        _cameraInformationBlockNeedUpdate = true;
    }
    
    void _onRender()
    {
        if (_cameraInformationBlock == nullptr)
            _cameraInformationBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(spk::Pipeline::uniform(spk::Pipeline::Uniform::Key(0, 0)));

        if (_cameraInformationBlockNeedUpdate == true)
        {
            DEBUG_LINE();
            _cameraInformationBlock->update();
            _cameraInformationBlockNeedUpdate = false;
        }
        _cameraInformationBlock->bind();

        for (size_t i = 0; i < _objects.size(); i++)
        {
            _objects[i].render();
        }
    }
    
    bool _onUpdate()
    {
        spk::Keyboard::Key keys[6] = {
            spk::Keyboard::Z,
            spk::Keyboard::Q,
            spk::Keyboard::S,
            spk::Keyboard::D,
            spk::Keyboard::X,
            spk::Keyboard::W
        };
        spk::Vector3 deltaPosition[6] = {
            _forward * 0.25f,
            _right * 0.25f,
            _forward * -0.25f,
            _right * -0.25f,
            spk::Vector3(0, 0.25f, 0),
            spk::Vector3(0, -0.25f, 0)
        };

        for (size_t i = 0; i < 6; i++)
        {
            if (spk::Keyboard::instance()->inputStatus(keys[i]) == spk::InputState::Pressed)
            {
                _position += deltaPosition[i];
                _initializeMVP();
            }
        }

        spk::Keyboard::Key keysAngle[2] = {
            spk::Keyboard::A,
            spk::Keyboard::E
        };
        float deltaAngle[2] = {
            5.0f,
            -5.0f
        };

        for (size_t i = 0; i < 2; i++)
        {
            if (spk::Keyboard::instance()->inputStatus(keysAngle[i]) == spk::InputState::Pressed)
            {
                _forward = _forward.rotate(spk::Vector3(0, deltaAngle[i], 0));
                _right = _right.rotate(spk::Vector3(0, deltaAngle[i], 0));
                _initializeMVP();
            }
        }

        return (false);
    }

    void _initializeMVP()
    {
        spk::Matrix4x4 projectionMatrix = spk::Matrix4x4::perspective(
				90.0f,
				1.0f,
				0.1f,
				100.0f
			);
            
        spk::Matrix4x4 viewMatrix = spk::Matrix4x4::lookAt(
				_position,
				_position + _forward,
				spk::Vector3(0, 1, 0)
			);
            
        if (_cameraInformationBlock == nullptr)
            _cameraInformationBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(spk::Pipeline::uniform(spk::Pipeline::Uniform::Key(0, 0)));
        _cameraInformationBlock->field(L"MVP") = projectionMatrix * viewMatrix * spk::Matrix4x4();
        _cameraInformationBlockNeedUpdate = true;
    }

public:
    GameEngineRendererWidget(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        _witnessWidget = addChildrenWidget<WitnessWidget>(L"WitnessWidget");
        _witnessWidget->activate();

        _objects.emplace_back(spk::Vector3(0, 0, 3));
        _objects.emplace_back(spk::Vector3(2, 0, 4));
        _objects.emplace_back(spk::Vector3(-1, 0, 5));
        _objects.emplace_back(spk::Vector3(0, 3, 6));

        _initializeMVP();    
    }

    ~GameEngineRendererWidget()
    {

    }
};

int main()
{
    spk::Application app(L"Playground", 900);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
  
    std::shared_ptr<GameEngineRendererWidget> renderer = app.addRootWidget<GameEngineRendererWidget>(L"GameEngineRendererWidget");
    renderer->setGeometry(0, app.size());
    renderer->activate();

    return (app.run());
}
