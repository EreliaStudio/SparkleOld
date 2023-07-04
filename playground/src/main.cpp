#include "playground.hpp"


int main()
{
	spk::StandardValue<int> tmp;

	tmp = -1;
	for (size_t i = 0; i < 4; i++)
	{
		tmp.save();
		tmp = i;
	}
	tmp.save();

	spk::cout << " --- Undoing ---" << std::endl;
	spk::cout << "Value : " << tmp << " should be 3" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp << " should be 2" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp << " should be 1" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp << " should be 0" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp << " should be -1" << std::endl;

	spk::cout << " --- Redoing ---" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp << " should be 0" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp << " should be 1" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp << " should be 2" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp << " should be 3" << std::endl;

	spk::cout << " --- Undoing ---" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp << " should be 2" << std::endl;

	spk::cout << " --- New branch ---" << std::endl;
	tmp = 42;
	tmp.save();
	spk::cout << "Value : " << tmp << " should be 42" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp << " should be 2" << std::endl;
	tmp.undo();
	spk::cout << "Value : " << tmp << " should be 1" << std::endl;

	spk::cout << " --- Redoing ---" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp << " should be 2" << std::endl;
	tmp.redo();
	spk::cout << "Value : " << tmp << " should be 42" << std::endl;


	return 0;
}