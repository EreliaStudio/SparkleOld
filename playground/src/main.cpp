#include "playground.hpp"

struct Client
{
	int id;

	friend std::wostream& operator << (std::wostream& p_os, const Client& p_client)
	{
		p_os << p_client.id;
		return (p_os);
	}
};

struct Employee
{
	std::wstring name;
	std::vector<Client> clients;

	friend std::wostream& operator << (std::wostream& p_os, const Employee& p_employee)
	{
		p_os << L"Name [" << p_employee.name << L"] - Client ID [";
		for (size_t i = 0; i < p_employee.clients.size(); i++)
		{
			if (i != 0)
				p_os << " - ";
			p_os << p_employee.clients[i];
		}
		return (p_os);
	}
};

int main() {
	spk::JSON::Unit data[5];

	data[0].set(true);
	data[1].set(10);
	data[2].set(15.5);
	data[3].set(L"Ceci est un test");
	data[4].set(data[1].as<int>() + data[2].as<double>());

	spk::JSON::Object object;

	object.addAttribute(L"Depth", 0);
	object.addAttribute(L"active", data[0]);
	object.addAttribute(L"level", data[1]);
	object.addAttribute(L"value", data[2]);
	object.addAttribute(L"name", data[3]);
	object.addAttribute(L"sum", data[4]);

	spk::JSON::Object objectHolder;

	objectHolder.addAttribute(L"Depth", 1);
	objectHolder.addAttribute(L"Children", object);

	spk::JSON::Object objectHolder2;

	objectHolder2.addAttribute(L"Depth", 2);
	objectHolder2.addAttribute(L"Children", objectHolder);

	const spk::JSON::Object* tmpObject = &objectHolder2;
	while (tmpObject->get(L"Depth").as<int>() > 0)
	{
		spk::cout << "Depth : " << tmpObject->get(L"Depth").as<int>() << std::endl;
		tmpObject = &(tmpObject->operator[](L"Children")[0]);
	}
	spk::cout << "TmpObject active = " << std::boolalpha << tmpObject->get(L"active").as<bool>() << std::endl;
	spk::cout << "TmpObject level = " << std::boolalpha << tmpObject->get(L"level").as<int>() << std::endl;
	spk::cout << "TmpObject value = " << std::boolalpha << tmpObject->get(L"value").as<double>() << std::endl;
	spk::cout << "TmpObject name = " << std::boolalpha << tmpObject->get(L"name").as<std::wstring>() << std::endl;
	spk::cout << "TmpObject sum = " << std::boolalpha << tmpObject->get(L"sum").as<double>() << std::endl;

    return 0;
}