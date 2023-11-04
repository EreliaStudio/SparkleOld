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
    spk::Font _font;
    std::shared_ptr<spk::Font::Atlas> _fontAtlas;
    spk::Image _image;

    int value = 0;

    void _onGeometryChange()
    {
        spk::Font::Atlas::GlyphData glyphData = _fontAtlas->glyph(L'j');

        std::vector<Unit> units = {
            {spk::Vector2(0.0f, -0.0f), glyphData.uvs[0]},
            {spk::Vector2(0.9f, -0.0f), glyphData.uvs[1]},
            {spk::Vector2(0.0f, -0.9f), glyphData.uvs[2]},
            {spk::Vector2(0.9f, -0.9f), glyphData.uvs[3]}};

        std::vector<unsigned int> indexes = {
            0, 2, 3, 3, 1, 0};

        _object->storage().vertices() << units << std::endl;
        _object->storage().indexes() << indexes << std::endl;
    }
    void _onRender()
    {
        if (value == 0)
            _fontAtlas->texture().bind(0);
        else
            _image.bind(0);

        _pipeline.uniform(L"textureID") << 0 << std::endl;
        _object->render();

        if (value == 0)
            _fontAtlas->texture().unbind();
        else
            _image.unbind();
    }

    bool _onUpdate()
    {
        if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::A) == spk::InputState::Pressed)
        {
            value = 0;
        }

        if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::Z) == spk::InputState::Pressed)
        {
            value = 1;
        }

        return (false);
    }

public:
    Test(const std::wstring &p_name) : spk::Widget::Interface(p_name),
                                       _pipeline(spk::ShaderModule(L"colorShader.vert"), spk::ShaderModule(L"colorShader.frag")),
                                       _object(_pipeline.createObject()),
                                       _font(L"Minecraft.ttf"),
                                       _fontAtlas(_font[spk::Font::Key(40, 2, spk::Font::OutlineType::Standard)]),
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
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

    std::shared_ptr<Test> test = app.addRootWidget<Test>(L"MainWidget");
    test->setGeometry(spk::Vector2Int(0, 0), app.size());
    test->activate();

    return (app.run());
}