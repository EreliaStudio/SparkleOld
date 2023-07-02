#include "playground.hpp"

int main()
{
	spk::JSON::File file(L"test_json.json");

	spk::cout << "File : " << std::endl << file[L"friends"] << std::endl;

  return (0);
}