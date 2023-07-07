#include "playground.hpp"

class Test : public spk::AbstractWidget
{
private:
	void _onRender()
	{

	}
	void _onGeometryChange()
	{

	}
	bool _onUpdate()
	{
	return (false);
	}
public:
	Test(std::wstring p_name) :
		spk::AbstractWidget(p_name)
	{

	}
};


int main() {
    auto widgetAtlas = spk::WidgetAtlas::instanciate();

    std::wcout << L"---------- Creation ----------" << std::endl;
    auto widget1 = new Test(L"Widget 1");
    auto widget2 = new Test(L"Widget 2");

    // Add children and grandchildren
    auto childWidget1 = widget1->addChildrenWidget<Test>(L"Child Widget 1");
    childWidget1->activate();
    auto grandChildWidget = childWidget1->addChildrenWidget<Test>(L"Grandchild Widget");
    grandChildWidget->activate();

    auto childWidget2 = widget2->addChildrenWidget<Test>(L"Child Widget 2");

    std::wcout << L"---------- After creation ----------" << std::endl;

    spk::WidgetAtlas::instance()->printInfo();

    std::wcout << L"---------- Activation ----------" << std::endl;

    widget1->activate();

    std::wcout << L"---------- After activation ----------" << std::endl;

    spk::WidgetAtlas::instance()->printInfo();

    std::wcout << L"---------- Deactivation ----------" << std::endl;

    widget1->deactivate();

    std::wcout << L"---------- After deactivation ----------" << std::endl;

    spk::WidgetAtlas::instance()->printInfo();

    std::wcout << L"---------- Before widget depth setting ----------" << std::endl;
    spk::WidgetAtlas::instance()->printInfo();

    widget1->setDepth(10);
    childWidget1->setDepth(15);
    grandChildWidget->setDepth(11);

    widget2->setDepth(1);
    childWidget2->setDepth(100);

    std::wcout << L"---------- After widget depth setting ----------" << std::endl;
    spk::WidgetAtlas::instance()->printInfo();

    return 0;
}
