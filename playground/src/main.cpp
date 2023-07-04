#include "playground.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <cstdlib>

class Memento
{
public:
    using Snapshot = spk::DataBuffer;

private:
    std::vector<Snapshot> _snapshots;
    int _index = -1;

    virtual Snapshot _onSave() = 0;
    virtual void _onLoad(const Snapshot& p_snapshot) = 0;

	void _load()
	{ 
		_snapshots[_index].reset();
        _onLoad(_snapshots[_index]);
	}

public:
    void save()
    {
        if (_index < _snapshots.size())
        {
            _snapshots.erase(_snapshots.begin() + _index, _snapshots.end());
        }
        _snapshots.push_back(_onSave());
        _index++;
    }

    void undo()
    {
		if (_index < 0)
			throw std::runtime_error("Can't undo cause no snapshot left");
        _index--;
		_load();
    }

    void redo()
    {
		if (_index >= _snapshots.size())
			throw std::runtime_error("Can't undo cause no snapshot left");
		_load();
        _index++;
    }
};


struct MyStruct : Memento
{
	Snapshot _onSave()
	{
		Snapshot result;

		spk::cout << "Saving value : " << value << std::endl;
		result << value;

		return result;
	}
	
	void _onLoad(const Snapshot& p_snapshot)
	{
		p_snapshot >> value;
	}
	
	int value;
};

    class Profiler
    {
        private:
		std::map<std::string, spk::Chronometer> _chronometers;

		public:
        void startChronometer(const std::string & p_name)
		{
			if (_chronometers.count(p_name) != 0 && _chronometers[p_name].duration() != 0)
				{
					 throw std::runtime_error("Tried to start an already active Chronometer");
				}
			_chronometers[p_name].start();
		}

        const long long stopChronometer(const std::string & p_name)
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
	Profiler test;


	test.startChronometer("test 1");
	Sleep(30);
	spk::cout << "First test of 30ms :"<< test.stopChronometer("test 1");


	test.startChronometer("test 2");
	test.startChronometer("test 3");
	Sleep(55);
	spk::cout << "Second test of 55ms :"<< test.stopChronometer("test 2");

	Sleep(80);
	spk::cout << "Third test of 135ms :"<< test.stopChronometer("test 3");

		test.startChronometer("test 1");
	Sleep(30);
	spk::cout << "Forth test of 30ms :"<< test.stopChronometer("test 1");

	return 0;
}
