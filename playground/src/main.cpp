#include "playground.hpp"

class MyWidget : public spk::AbstractWidget
{
private:
	size_t nb = 0;	
	void _onRender()
	{

		if (nb == 0)
		{
 	spk::Debug::Profiler::instance()->startChronometer(L"test 1");
	spk::TimeMetrics::instance()->sleepAtLeast(30);
	spk::cout << "First test of 30ms :"<< spk::Debug::Profiler::instance()->stopChronometer(L"test 1") << std::endl;

	spk::Debug::Profiler::instance()->startChronometer(L"test 2");
	spk::Debug::Profiler::instance()->startChronometer(L"test 3");
	spk::TimeMetrics::instance()->sleepAtLeast(55);
	spk::cout << "Second test of 55ms :"<< spk::Debug::Profiler::instance()->stopChronometer(L"test 2") << std::endl;

	spk::TimeMetrics::instance()->sleepAtLeast(80);
	spk::cout << "Third test of 135ms :"<< spk::Debug::Profiler::instance()->stopChronometer(L"test 3") << std::endl;

	spk::Debug::Profiler::instance()->startChronometer(L"test 1");
	spk::TimeMetrics::instance()->sleepAtLeast(30);
	spk::cout << "Forth test of 30ms :"<< spk::Debug::Profiler::instance()->stopChronometer(L"test 1") << std::endl;
		}
		nb++;
	}

	void _onGeometryChange()
	{

	}

	bool _onUpdate()
	{
		
		return (false);
	}

public:
	MyWidget() : spk::AbstractWidget(L"Ceci est un test")
	{
		spk::Debug::Profiler::instanciate();
	}
};

int main()
{
	spk::Application app(L"myApp", spk::Vector2Int(400, 400));

	MyWidget* ourWidget = app.centralWidget()->addChildrenWidget<MyWidget>();
	ourWidget->activate();

	return app.run();
}
