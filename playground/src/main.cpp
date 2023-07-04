#include "playground.hpp"

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
        if (_index + 1< _snapshots.size())
        {
            _snapshots.erase(_snapshots.begin() + _index + 1, _snapshots.end());
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
		if ((_index + 1) >= _snapshots.size())
			throw std::runtime_error("Can't undo cause no snapshot left");
        _index++;
		_load();
    }
};


struct MyStruct : Memento
{
	int value;
	Snapshot _onSave()
	{
		Snapshot result;

		result << value;

		return result;
	}
	
	void _onLoad(const Snapshot& p_snapshot)
	{
		p_snapshot >> value;
	}
	
};

int main()
{
	MyStruct tmp;

	tmp.value = -1;
	for (size_t i = 0; i < 4; i++)
	{
		tmp.save();
		tmp.value = i;
	}
	tmp.save();

	spk::cout << " --- Undoing ---" << std::endl;
	spk::cout << "Value : " << tmp.value << " should be 3" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << " should be 2" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << " should be 1" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << " should be 0" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << " should be -1" << std::endl;

	spk::cout << " --- Redoing ---" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp.value << " should be 0" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp.value << " should be 1" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp.value << " should be 2" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp.value << " should be 3" << std::endl;

	spk::cout << " --- Undoing ---" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << " should be 2" << std::endl;

	spk::cout << " --- New branch ---" << std::endl;
	tmp.value = 42;
	tmp.save();
	spk::cout << "Value : " << tmp.value << " should be 42" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << " should be 2" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << " should be 1" << std::endl;

	spk::cout << " --- Redoing ---" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp.value << " should be 2" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp.value << " should be 42" << std::endl;


	return 0;
}