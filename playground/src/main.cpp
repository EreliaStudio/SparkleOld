#include "playground.hpp"

class Test : public spk::Widget::Interface
{
	registerClass(Test, RegisterName);
	registerClass(Test, AnotherRegisterName);

private:
	void _onGeometryChange()
	{

	}

	void _onRender()
	{

	}

	bool _onUpdate()
	{
		return (false);
	}

public:
	Test(const std::wstring& p_name) : 
		spk::Widget::Interface(p_name)
	{

	}

	~Test()
	{

	}
	
	Test(const std::wstring& p_name, const spk::JSON::Object& p_object) : 
		spk::Widget::Interface(p_name, p_object)
	{
		DEBUG_LINE();
	}
};


int main()
{
	spk::Application app(L"Coucou", 400, spk::Application::Configuration(true, true));
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

	spk::Widget::Canvas* canvas = app.addCanvas(L"canvasConfigurationFile.json");
	canvas->activate();

	return (app.run());
};