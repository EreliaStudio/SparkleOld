#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:
	spk::WidgetComponent::Box _box;

	void _onGeometryChange()
	{
        DEBUG_LINE();
		_box.setGeometry(area());
        DEBUG_LINE();
		_box.setDepth(1.0f);
        DEBUG_LINE();
	}

	void _onRender()
	{		
		_box.render();
	}
	
	bool _onUpdate()
	{
		return (false);
	}

public:
	Test(const std::wstring &p_name) : spk::Widget::Interface(p_name)
	{

	}
	
	~Test()
	{

	}
};

int main()
{
	spk::Application app(L"Playground", 400);
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

	std::shared_ptr<Test> test = app.addRootWidget<Test>(L"Test");
	test->setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(400, 400));
	test->activate();

	return (app.run());
};