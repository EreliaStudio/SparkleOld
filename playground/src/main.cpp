#include "playground.hpp"

#include "external_libraries/glm/glm.hpp"
#include "external_libraries/glm/gtc/matrix_transform.hpp"

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

class GameEngineRendererWidget : public spk::Widget::Interface
{
private:
    std::shared_ptr<WitnessWidget> _witnessWidget;

    std::shared_ptr<spk::Pipeline> _renderingPipeline;
    std::shared_ptr<spk::Pipeline::Object> _renderingObject;
    std::shared_ptr<spk::Pipeline::UniformBlock> _cameraInformationBlock;

    std::shared_ptr<spk::Mesh> _mesh;

    spk::Matrix4x4 _MVP;

    void _onGeometryChange()
    {
        _witnessWidget->setGeometry(spk::Vector2Int(10, 10), spk::Vector2UInt(800, 150));
    }
    
    void _onRender()
    {
        _cameraInformationBlock->field(L"MVP") = _MVP;
        _renderingObject->render();
    }
    
    bool _onUpdate()
    {
        return (false);
    }

    void _initializeMVP()
    {
        auto projectionMatrix = spk::Matrix4x4::perspective(
				90.0f,
				1.0f,
				0.1f,
				100.0f
			);

        auto viewMatrix = spk::Matrix4x4::lookAt(
				spk::Vector3(0, 0, 13),
				spk::Vector3(0, 0, -1),
				spk::Vector3(0, 1, 0)
			);

		_MVP = (projectionMatrix * viewMatrix * spk::Matrix4x4());

        _cameraInformationBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(_renderingPipeline->uniform(L"CameraInformation"));

        for (size_t i = 0; i < _mesh->points().size(); i++)
        {
            spk::cout << "Point [" << i << "] : " << _mesh->points()[i] << " -> " << _MVP * _mesh->points()[i] << std::endl;
        }
    }

    void _initializeMesh()
    {
        _mesh = std::make_shared<spk::Mesh>();

        _mesh->points().push_back(spk::Vector3(-1, -1, 0));
        _mesh->points().push_back(spk::Vector3(1, -1, 0));
        _mesh->points().push_back(spk::Vector3(-1, 1, 0));
        _mesh->points().push_back(spk::Vector3(1, 1, 0));

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
    GameEngineRendererWidget(const std::wstring& p_name) : 
        spk::Widget::Interface(p_name)
    {
        _witnessWidget = addChildrenWidget<WitnessWidget>(L"WitnessWidget");
        _witnessWidget->activate();

        _renderingPipeline = std::make_shared<spk::Pipeline>(spk::ShaderModule(L"testShader.vert"), spk::ShaderModule(L"testShader.frag"));
        _renderingObject = _renderingPipeline->createObject();

        _initializeMesh();

        _initializeRenderData();

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
