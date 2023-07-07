#include "playground.hpp"

class Test : public spk::AbstractWidget
{
private:
	void _onRender()
	{
        spk::cout << L"Render widget [" << name() <<  L"] at depth [" << depth() << L"]" << std::endl;
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


int main()
{
    spk::Application app(L"Coucou", 400);

    Test* widgetA = app.addRootWidget<Test>(L"WidgetA");
    widgetA->activate();
    
    Test* widgetA_A = widgetA->addChildrenWidget<Test>(L"WidgetA_A");
    widgetA_A->activate();
    Test* widgetA_A_A = widgetA_A->addChildrenWidget<Test>(L"WidgetA_A_A");
    widgetA_A_A->activate();
    Test* widgetA_B = widgetA->addChildrenWidget<Test>(L"WidgetA_B");
    widgetA_B->activate();
    Test* widgetA_C = widgetA->addChildrenWidget<Test>(L"WidgetA_C");
    widgetA_C->activate();
    Test* widgetA_C_A = widgetA_C->addChildrenWidget<Test>(L"WidgetA_C_A");
    widgetA_C_A->activate();

    Test* widgetB = app.addRootWidget<Test>(L"WidgetB");
    widgetB->activate();
    Test* widgetB_A = widgetB->addChildrenWidget<Test>(L"WidgetB_A");
    widgetB_A->activate();
    
    Test* widgetC = app.addRootWidget<Test>(L"WidgetC");
    widgetC->activate();
    Test* widgetC_A = widgetC->addChildrenWidget<Test>(L"WidgetC_A");
    widgetC_A->activate();
    Test* widgetC_B = widgetC->addChildrenWidget<Test>(L"WidgetC_B");
    widgetC_B->activate();
    
    widgetA->setDepth(10);
        widgetA_A->setDepth(3);
            widgetA_A_A->setDepth(5);
        widgetA_B->setDepth(10);
        widgetA_C->setDepth(11);
            widgetA_C_A->setDepth(9);
    widgetB->setDepth(20);
        widgetB_A->setDepth(25);
    widgetC->setDepth(0);
        widgetC_A->setDepth(2);
        widgetC_B->setDepth(4);

    return (app.run());
}
