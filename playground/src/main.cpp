#include <memory>
#include "inherence_object.hpp"
#include <iostream>

class BabyObject: public ft::InherenceObject<BabyObject>
{
	public:
		std::string name;

		BabyObject(const std::string & p_name)
		: ft::InherenceObject<BabyObject>()
		, name(p_name)
		{}

		~BabyObject() {}
};

void family_creation(BabyObject & elder)
{
	BabyObject first("Ben1");
	first.setParent(&elder);

	BabyObject second("Max1");
	elder.addChild(&second);
}

int main()
{
	std::cout << "Hey" << std::endl;

	BabyObject elder("Oldi");

	elder.setBirthCallback([](BabyObject *child) {
		std::cout << "Birth of " << child->name << std::endl;
	});

	elder.setDeathCallback([](BabyObject *child) {
		std::cout << "Death of " << child->name << std::endl;
	});

	BabyObject first("Ben");
	first.setParent(&elder);

	BabyObject second("Max");
	// BabyObject *second = new BabyObject("Max");
	// second->setParent(elder);
	elder.addChild(&second);

	family_creation(elder);

	std::cout << "Children number: " << elder.childrens().size() << std::endl;

	// delete elder;
	// delete first;
	// delete second;

	return 0;
}