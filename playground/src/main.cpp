#include "playground.hpp"

<<<<<<< HEAD
<<<<<<< HEAD
int main()
{
	spk::DataBuffer dataBuffer;



	int valueA1 = 42;
	int valueA2 = 12;
	std::wstring valueB1 = L"Coucou";

	dataBuffer << valueA1;
	dataBuffer << valueA2;
	dataBuffer << valueB1;



	int extractedValueA;
	int extractedValueA2;
	std::wstring extractedValueB;

	dataBuffer >> extractedValueA;
	dataBuffer >> extractedValueA2;
	dataBuffer >> extractedValueB;
	
	spk::cout << "Value A : " << extractedValueA << " - " << extractedValueA2 << " - " << extractedValueB << std::endl;
=======
class Object
=======
class Memento
>>>>>>> f44fb43 (Testing a new Memento implem)
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
<<<<<<< HEAD
	intMementoTest();
	customMemtoTest();
>>>>>>> 76525da (✨✅ Improvement of Memento)
=======
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
>>>>>>> f44fb43 (Testing a new Memento implem)

	return 0;
}