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

    std::shared_ptr<WitnessWidget> _witnessWidget;

    std::vector<MeshRenderer> _objects;

    spk::Matrix4x4 _MVP;

    bool state = false;

    void _onGeometryChange()
    {
        _witnessWidget->setGeometry(spk::Vector2Int(10, 10), spk::Vector2UInt(800, 150));
    }
    
    void _onRender()
    {
        if (_cameraInformationBlock == nullptr)
            _cameraInformationBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(spk::Pipeline::uniform(spk::Pipeline::Uniform::Key(0, 0)));

        _cameraInformationBlock->field(L"MVP") = _MVP;

        for (size_t i = 0; i < _objects.size(); i++)
        {
            
            _objects[i].render();
        }

        if (state == false)
        {
            for (size_t i = 0; i < _objects.size(); i++)
            {
                if (i != 0)
                    spk::cout << std::endl << std::endl;
                for (size_t j = 0; j < _objects[i].mesh()->points().size(); j++)
                {
                    spk::cout << "Point [" << (_objects[i].mesh()->points()[j] + _objects[i].position()) << "] = [" << _MVP * (_objects[i].mesh()->points()[j] + _objects[i].position()) << "]" << std::endl;
                }
            }
            
            state = true;
        }
    }
    
    bool _onUpdate()
    {
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
				spk::Vector3(0, 0, -1),
				spk::Vector3(0, 0, 0),
				spk::Vector3(0, 1, 0)
			);

		_MVP = (projectionMatrix * viewMatrix * spk::Matrix4x4());
    }

public:
    GameEngineRendererWidget(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        _witnessWidget = addChildrenWidget<WitnessWidget>(L"WitnessWidget");
        _witnessWidget->activate();

        _initializeMVP();    

        _objects.emplace_back(spk::Vector3(0, 0, 0));
        _objects.emplace_back(spk::Vector3(0, 0, 5));
        _objects.emplace_back(spk::Vector3(0, 0, 10));
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
