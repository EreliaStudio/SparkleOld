#include "playground.hpp"

int main()
{
	spk::JSON::Object object;
	std::vector<spk::JSON::Object> tmpArray;
	spk::JSON::Object holder;

	object.addAttribute(L"Depth", 0);

	holder.addObjectArray(L"Childrens");

	for (size_t i = 0; i < 5; i++)
	{
		object.get(L"Depth").set<int>(4 - i);
		holder.getObjectArray(L"Childrens").push_back(object);
	}

	for (size_t i = 0; i < holder.getObjectArray(L"Childrens").size(); i++)
	{
		spk::cout << "Object [" << i << "] -> Depth [" << holder.getObjectArray(L"Childrens")[i].get(L"Depth").as<int>() << "]" << std::endl;
	}

	return 0;
}