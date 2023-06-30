#include "playground.hpp"

class Object
{
public:
	using Unit = std::variant<bool, int, double, std::wstring, Object*, std::nullptr_t>;
	using ContentType = std::variant<Unit, std::map<std::wstring, Object*>, std::vector<Object*>>;

private:
	bool _initialized;
	ContentType _content;

public:
	Object()
	{
		_initialized = false;
	}

	Object* addAttribute(const std::wstring& p_key)
	{
		if (_initialized == false)
		{
			_content = std::map<std::wstring, Object*>();
			_initialized = true;
		}

		Object* result = new Object();
		std::get<std::map<std::wstring, Object*>>(_content)[p_key] = result;
		return (result);
	}

	void list()
	{
		auto& map = std::get<std::map<std::wstring, Object*>>(_content);

		bool first = true;

		spk::cout << "Content keys : ";
		for (auto& element : map)
		{
			if (first == false)
				spk::cout << " - ";
			spk::cout << element.first;
			first = false;
		}
		spk::cout << std::endl;
	}

	Object* operator [] (const std::wstring& p_key)
	{
		Object* result = std::get<std::map<std::wstring, Object*>>(_content).at(p_key);

		if (std::holds_alternative<Unit>(result->_content) == false)
			return (result);
		else
		{
			if (std::holds_alternative<Object *>(std::get<Unit>(result->_content)) == false)
				return (result);
			else	
				return (result->as<Object*>());
		}
	}

	const Object* operator [] (const std::wstring& p_key) const
	{
		Object* result = std::get<std::map<std::wstring, Object*>>(_content).at(p_key);

	/*
		Forced to add this to prevent user from writing as<Object*>() after getting an object from the map
		it's not mandatory, but it's easier to read for the user with this.

		If not keeped, we coult simply :
		return (std::get<std::map<std::wstring, Object*>>(_content).at(p_key));
	*/
		if (std::holds_alternative<Unit>(result->_content) == false)
			return (result);
		else
		{
			if (std::holds_alternative<Object *>(std::get<Unit>(result->_content)) == false)
				return (result);
			else	
				return (result->as<Object*>());
		}
	}

	Object* push_back()
	{
		if (_initialized == false)
		{
			_content = std::vector<Object*>();
			_initialized = true;
		}
		Object* result = new Object();
		std::get<std::vector<Object*>>(_content).push_back(result);
		return (result);
	}

	Object* operator[](size_t p_index)
	{
		return (std::get<std::vector<Object*>>(_content)[p_index]);
	}

	const Object* operator[](size_t p_index) const
	{
		return (std::get<std::vector<Object*>>(_content)[p_index]);
	}

	size_t size() const 
	{
		return (std::get<std::vector<Object*>>(_content).size());
	}

	size_t count(const std::wstring& p_key) const
	{
		return (std::get<std::map<std::wstring, Object*>>(_content).count(p_key));
	}

	template<typename TType>
	void set(const TType& p_value)
	{
		if (_initialized == false)
		{
			_content = Unit();
			_initialized = true;
		}

		std::get<Unit>(_content) = p_value;
	}

	template<typename TType>
	const TType& as()
	{
		return (std::get<TType>(std::get<Unit>(_content)));
	}
};

int main()
{
	Object* weapon = new Object();
	weapon->addAttribute(L"Name")->set<std::wstring>(L"Sword");
	weapon->addAttribute(L"Damage")->set<int>(10);

	Object tmp;

	spk::cout << __LINE__ << std::endl;

	Object* objectA = tmp.push_back();
	Object* objectB = tmp.push_back();
	Object* objectC = tmp.push_back();

	spk::cout << __LINE__ << std::endl;

	objectA->set<int>(10);
	spk::cout << __LINE__ << std::endl;
	objectB->set<std::wstring>(L"Ceci est un test");
	spk::cout << __LINE__ << std::endl;

	spk::cout << __LINE__ << std::endl;
	objectC->addAttribute(L"Name")->set<std::wstring>(L"NameOfTheObject"); 
	spk::cout << __LINE__ << std::endl;
	objectC->addAttribute(L"Level")->set<int>(10);
	spk::cout << __LINE__ << std::endl;
	objectC->addAttribute(L"Weapon")->set<Object*>(weapon);
	spk::cout << __LINE__ << std::endl;

	spk::cout << "tmp A : " << std::endl;
	spk::cout << "tmp[0] = " << tmp[0]->as<int>() << std::endl;
	spk::cout << "tmp[1] = " << tmp[1]->as<std::wstring>() << std::endl;
	spk::cout << "tmp[2] = " << tmp[2]->operator[](L"Name")->as<std::wstring>() << " / " << tmp[2]->operator[](L"Level")->as<int>() << std::endl;
	spk::cout << "         and is equiped with -> " << tmp[2]->operator[](L"Weapon")->operator[](L"Name")->as<std::wstring>() << std::endl;

	return (0);
}