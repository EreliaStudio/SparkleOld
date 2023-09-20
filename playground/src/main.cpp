#include "sparkle.hpp"

class Test : public spk::Widget::Interface
{
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