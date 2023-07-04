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

int main()
{
	MyStruct tmp;

	tmp.value = -1;
	for (size_t i = 0; i < 4; i++)
	{
		tmp.save();
		tmp.value = i;
	}

	spk::cout << "Value : " << tmp.value << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp.value << std::endl;

	return 0;
}
