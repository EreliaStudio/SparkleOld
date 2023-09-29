#include "sparkle.hpp"

class Test : public spk::Widget::Interface
{
private:
	spk::GraphicalAPI::Pipeline _pipeline;
	std::shared_ptr<spk::GraphicalAPI::Pipeline::Object> _object;

	void _onGeometryChange()
	{
		std::vector<spk::GraphicalAPI::Pipeline::Object::Storage::Unit<spk::Vector2>> datas = {
			{spk::Vector2( 0.0f,  1.0f)},
			{spk::Vector2(-1.0f, -1.0f)},
			{spk::Vector2( 1.0f, -1.0f)}
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
		_object->pushConstants()[L"color"] << spk::Color(255, 255, 255);
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