#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:
	int count = 0;
	spk::WidgetComponent::Box _box;

	void _onGeometryChange()
	{
		_box.setGeometry(area(), 10);
		_box.setDepth(depth());
	}

	void _onRender()
	{		
		if (count == 0)
		spk::cout << "Rendering [" << name() << "]" << std::endl;
		_box.render();
		count++;
	}
	
	bool _onUpdate()
	{
		return (false);
	}

public:
	Test(const std::wstring &p_name) : spk::Widget::Interface(p_name)
	{

	}

	void setColors(spk::Color p_colorA, spk::Color p_colorB)
	{
		_box.setColors(p_colorA, p_colorB);
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
	test->setColors(spk::Color(180, 20, 20, 255), spk::Color(100, 20, 20, 255));
	test->setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(400, 400));
	test->activate();

	std::shared_ptr<Test> test2 = test->addChildrenWidget<Test>(L"TestChildren");
	test2->setColors(spk::Color(20, 20, 180, 255), spk::Color(20, 20, 100, 255));
	test2->setGeometry(spk::Vector2Int(30, 30), spk::Vector2UInt(340, 340));
	test2->activate();

	std::shared_ptr<Test> test3 = test2->addChildrenWidget<Test>(L"TestChildren2");
	test3->setColors(spk::Color(20, 180, 20, 255), spk::Color(20, 100, 20, 255));
	test3->setGeometry(spk::Vector2Int(130, 30), spk::Vector2UInt(280, 280));
	test3->activate();

	return (app.run());
};