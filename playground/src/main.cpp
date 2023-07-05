#include "playground.hpp"


    class Profiler
    {
        private:
		std::map<std::wstring, spk::Chronometer> _chronometers;

		public:
		Profiler()
		{
			if (spk::Singleton<spk::TimeMetrics>::instance() == nullptr)
			{
				throw std::runtime_error("Profiler can't be launched without an application");
			}
		}

        void startChronometer(const std::wstring & p_name)
		{
			if (_chronometers.count(p_name) != 0 && _chronometers[p_name].duration() != 0)
			{
				throw std::runtime_error("Tried to start an already active Chronometer");
			}
			_chronometers[p_name].start();
		}

        const long long stopChronometer(const std::wstring & p_name)
		{
			if (_chronometers.count(p_name) == 0 )
			{
				throw std::runtime_error("This Chronometer does not exist ");
			}
			if (_chronometers[p_name].duration() == 0)
			{
				throw std::runtime_error("This Chronometer is not started ");
			}		
			return(_chronometers[p_name].stop());
		}
		
    };

int main()
{
	spk::Application app(L"myApp", spk::Vector2Int(400,400));


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