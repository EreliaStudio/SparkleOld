#include "sparkle.hpp"
#include "graphics/pipeline/opengl/spk_graphical_api_pipeline_basic_functions.hpp"

class Test : public spk::Widget::Interface
{
private:
	spk::GraphicalAPI::Pipeline _pipeline;
	std::shared_ptr<spk::GraphicalAPI::Pipeline::Object> _object;

	void _onGeometryChange()
	{
		std::vector<spk::GraphicalAPI::Pipeline::Object::Storage::Unit<spk::Vector2, spk::Color>> datas = {
			{spk::Vector2( 0.0f,  1.0f), spk::Color(255, 0, 0)},
			{spk::Vector2(-1.0f, -1.0f), spk::Color(0, 255, 0)},
			{spk::Vector2( 1.0f, -1.0f), spk::Color(0, 0, 255)}
		};
		std::vector<unsigned int> indexes = {
			0, 1, 2
		};
		
		_object->storage() << datas;
		_object->indexes() << indexes;

		_object->push();
	}

	void _onRender()
	{
		_object->render();
	}
	
	bool _onUpdate()
	{
		return (false);
	}

public:
	Test(const std::wstring& p_name) :
		spk::Widget::Interface(p_name),
		_pipeline(L"colorShader.vert", L"colorShader.frag"),
		_object(_pipeline.createObject())
	{
	
	}
	~Test()
	{

	}
};

int main()
{
	spk::Application app(L"Coucou", 400);
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Qwerty);

	std::shared_ptr<Test> test = app.addRootWidget<Test>(L"Test");
	test->setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(400, 400));
	test->activate();

	return (app.run());
};