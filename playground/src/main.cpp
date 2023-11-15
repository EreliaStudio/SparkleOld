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
        if (_cameraInformationBlock == nullptr)
            _cameraInformationBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(_renderingPipeline->uniform(L"CameraInformation"));

        _cameraInformationBlock->field(L"MVP") = _MVP;
        _renderingObject->render();
    }
    
    bool _onUpdate()
    {
        return (false);
    }

    void testMVPValue(const float& p_near, const float& p_far)
    {
        const spk::Vector3 position = spk::Vector3(0, 0, 0);
        const spk::Vector3 direction = spk::Vector3(0, 0, 1);
        std::vector<spk::Vector3> tmpPoints;

        for (float value = 0.001f / p_far; value <= p_far * 100; value *= 10)
        {
            tmpPoints.push_back(spk::Vector3(0, 0, value));
        }

        auto projectionMatrix = spk::Matrix4x4::perspective(
				90.0f,
				1.0f,
				p_near,
				p_far
			);

        auto viewMatrix = spk::Matrix4x4::lookAt(
				position,
				position + direction * p_far,
				spk::Vector3(0, 1, 0)
			);

		spk::Matrix4x4 tmp = (projectionMatrix * viewMatrix * spk::Matrix4x4());

        spk::cout << "Matrix Near = " << p_near << " / Far = " << p_far << " / Pos = " << position << " / Direction = " << direction << " :"  << tmp << std::endl;
        for (size_t i = 0; i < tmpPoints.size(); i++)
        {
            spk::cout << "Point [" << std::setw(2) << i << "] : [" << tmpPoints[i] << "]   ->   [" << tmp * tmpPoints[i] << "]" << std::endl;
        }

        spk::cout << std::endl << std::endl;
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
				spk::Vector3(0, 0, 8),
				spk::Vector3(0, 0, -1),
				spk::Vector3(0, 1, 0)
			);

		_MVP = (projectionMatrix * viewMatrix * spk::Matrix4x4());
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
    
        testMVPValue(0.1f, 1.0f);
        testMVPValue(0.1f, 10.0f);
        testMVPValue(0.1f, 100.0f);
        testMVPValue(0.1f, 1000.0f);
        testMVPValue(0.1f, 10000.0f);
    
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
