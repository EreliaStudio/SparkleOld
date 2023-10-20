#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:
	spk::Pipeline _pipeline;
	std::shared_ptr<spk::Pipeline::Object> _object;

	float rotation = 0;

	void _onGeometryChange()
	{
		std::vector<spk::Vector2> datas = {
			spk::Vector2( 0.0f,  1.0f),
			spk::Vector2(-1.0f, -1.0f),
			spk::Vector2( 1.0f, -1.0f)
		};
		
		std::vector<unsigned int> indexes = {
			0, 1, 2
		};
		
		_object->storage().vertices() << datas << std::endl;
		_object->storage().indexes() << indexes << std::endl;
	}

	void _onRender()
	{
		_object->render();
	}
	
	bool _onUpdate()
	{
		rotation += spk::TimeMetrics::instance()->deltaTime();

		_object->pushConstants()[L"rotation"] << spk::Matrix4x4::rotationMatrix(spk::Vector3(0, 0, rotation / 1000)) << std::endl;

		return (false);
	}

public:
	Test(const std::wstring &p_name) : spk::Widget::Interface(p_name),
		_pipeline(spk::ShaderModule("colorShader.vert"), spk::ShaderModule("colorShader.frag")),
		_object(_pipeline.createObject())
	{
		_pipeline.uniform(L"colorUniformBlock") << spk::Color(255, 0, 0) << std::endl;
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