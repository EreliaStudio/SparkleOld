#include "playground.hpp"

class Object
{
private:
	int _toRemember;
	spk::Vector2Int _toRememberToo;

	int _notImportant;

public:
	Object(int p_toRemember, spk::Vector2Int p_toRememberToo, int p_notImportant):
		_toRemember(p_toRemember),
		_toRememberToo(p_toRememberToo),
		_notImportant(p_notImportant)
	{
	}

	friend std::wostream& operator<<(std::wostream& p_os, const Object& p_object)
	{
		p_os << L"Object: " << p_object._toRemember << L", " << p_object._toRememberToo << L", " << p_object._notImportant;
		return p_os;
	}

	friend spk::DataBuffer& operator<<(spk::DataBuffer& p_buffer, const Object& p_object)
	{
		p_buffer << p_object._toRemember << p_object._toRememberToo;
		return p_buffer;
	}

	friend spk::DataBuffer& operator>>(spk::DataBuffer& p_buffer, Object& p_object)
	{
		p_buffer >> p_object._toRemember >> p_object._toRememberToo;
		return p_buffer;
	}
};

template <typename TType>
void updateValue(spk::Memento<TType>& p_memento,
	TType& p_value, const TType& p_newValue)
{
	p_value = p_newValue;
	p_memento.save(p_value);
	spk::cout << L"Saved at: " << p_value << std::endl;
}

template <typename TType>
void undoValue(spk::Memento<TType>& p_memento, TType& p_value)
{
	p_memento.undo(p_value);
	spk::cout << L"\tUndo: " << p_value << std::endl;
}

template <typename TType>
void redoValue(spk::Memento<TType>& p_memento, TType& p_value)
{
	p_memento.redo(p_value);
	spk::cout << L"\tRedo: " << p_value << std::endl;
}

void intMementoTest()
{
	spk::cout << L"\nIntMementoTest" << std::endl;

	spk::Memento<int> memento;
	int state = 0;

	updateValue(memento, state, 1);
	updateValue(memento, state, 2);

	spk::cout << L"Current state: " << state << std::endl;

	undoValue(memento, state);
	try
	{
		undoValue(memento, state);
	}
	catch (const std::exception& e)
	{
		spk::cout << L"\t\t" << e.what() << std::endl;
	}
	redoValue(memento, state);

	undoValue(memento, state);
	redoValue(memento, state);
	try
	{
		redoValue(memento, state);
	}
	catch (const std::exception& e)
	{
		spk::cout << L"\t\t" << e.what() << std::endl;
	}
}

void customMemtoTest()
{
	spk::Memento<Object> memento;
	Object state(0, {0, 0}, 0);

	updateValue(memento, state, Object(1, {1, 1}, 1));
	memento.reset();
	updateValue(memento, state, Object(2, {2, 2}, 2));
	updateValue(memento, state, Object(3, {3, 3}, 3));

	spk::cout << L"Current state: " << state << std::endl;
	undoValue(memento, state);

	redoValue(memento, state);
}

int main()
{
	intMementoTest();
	customMemtoTest();

	return 0;
}
