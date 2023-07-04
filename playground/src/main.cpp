#include "playground.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <cstdlib>

class Profiler : public spk::Singleton<Profiler>
{
	friend class spk::Singleton<Profiler>;

private:
	std::map<std::wstring, spk::Chronometer> _chronometers;

	Profiler() = default;

public:

	void startChronometer(const std::wstring &p_name)
	{
		_chronometers[p_name].start();
	}

	void resumeChronometer(const std::wstring &p_name)
	{
		if (_chronometers.count(p_name) == 0)
		{
			throw std::runtime_error("This Chronometer does not exist ");
		}
		_chronometers[p_name].resume();
	}

	const long long stopChronometer(const std::wstring &p_name)
	{
		if (_chronometers.count(p_name) == 0)
		{
			throw std::runtime_error("This Chronometer does not exist ");
		}
		return (_chronometers[p_name].stop());
	}
};

class MyWidget : public spk::AbstractWidget
{
private:
	size_t nb = 0;
	void _onRender()
	{	
		if ((nb % 1000) == 0)
		{
			if (nb != 0)
			{
				spk::cout << "Chronometer time : " << Profiler::instance()->stopChronometer(L"RenderChronometer") << std::endl;
			}
			Profiler::instance()->startChronometer(L"RenderChronometer");
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
		Profiler::instanciate();
	}
};

int main()
{
	spk::Application app(L"myApp", spk::Vector2Int(400, 400));

	MyWidget* ourWidget = app.centralWidget()->addChildrenWidget<MyWidget>();
	ourWidget->activate();

	return app.run();
}

// int main()
// {

// 	Profiler test;

// 	test.startChronometer(L"test 1");
// 	spk::TimeMetrics::instance()->sleep(30);
// 	spk::cout << "First test of 30ms :"<< test.stopChronometer(L"test 1") << std::endl;

// 	test.startChronometer(L"test 2");
// 	test.startChronometer(L"test 3");
// 	spk::TimeMetrics::instance()->sleep(55);
// 	spk::cout << "Second test of 55ms :"<< test.stopChronometer(L"test 2") << std::endl;

// 	spk::TimeMetrics::instance()->sleep(80);
// 	spk::cout << "Third test of 135ms :"<< test.stopChronometer(L"test 3") << std::endl;

// 	test.startChronometer(L"test 1");
// 	spk::TimeMetrics::instance()->sleep(30);
// 	spk::cout << "Forth test of 30ms :"<< test.stopChronometer(L"test 1") << std::endl;

// 	return 0;
// }
