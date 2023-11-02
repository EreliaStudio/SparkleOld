#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:

    struct Unit
    {
        spk::Vector2 position;
        spk::Vector2 uv;
    };

    spk::Pipeline _pipeline;
    std::shared_ptr<spk::Pipeline::Object> _object;
    spk::Image _image;  

    void _onGeometryChange()
    {
        std::vector<Unit> units = {
            {spk::Vector2(-1,  1), spk::Vector2(0, 0)},
            {spk::Vector2( 1,  1), spk::Vector2(1, 0)},
            {spk::Vector2(-1, -1), spk::Vector2(0, 1)},
            {spk::Vector2( 1, -1), spk::Vector2(1, 1)},
        };

        std::vector<unsigned int> indexes = {
            0, 2, 3, 3, 1, 0
        };

        _object->storage().vertices() << units << std::endl;
        _object->storage().indexes() << indexes << std::endl;
    }
    void _onRender()
    {
        _image.bind(0);
        _pipeline.uniform(L"textureID") << 0 << std::endl;
        _object->render();
        _image.unbind();
    }

    bool _onUpdate()
    {
        return (false);
    }

public:
    Test(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _pipeline(spk::ShaderModule(L"colorShader.vert"), spk::ShaderModule(L"colorShader.frag")),
        _object(_pipeline.createObject()),
        _image(L"imageTest.png")
    {

    }

    ~Test()
    {

    }
};

int main()
{
    spk::Application app(L"Playground", 800);

    std::shared_ptr<Test> test = app.addRootWidget<Test>(L"MainWidget");
    test->setGeometry(spk::Vector2Int(0, 0), app.size());
    test->activate();

    return (app.run());
}