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

	Object& addAttribute(const std::wstring& p_key)
	{
		if (_initialized == false)
		{
			_content = std::map<std::wstring, Object*>();
			_initialized = true;
		}

		Object* result = new Object();
		std::get<std::map<std::wstring, Object*>>(_content)[p_key] = result;
		return (*result);
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

	/*
		Forced to do this to prevent user from writing as<Object*>() after getting an object from the map
		it's not mandatory, but it's easier to read for the user with this.

		If not keeped, we coult simply :
		return (std::get<std::map<std::wstring, Object*>>(_content).at(p_key));
	*/
	Object& operator [] (const std::wstring& p_key)
	{
		Object* result = std::get<std::map<std::wstring, Object*>>(_content).at(p_key);

		if (std::holds_alternative<Unit>(result->_content) == false)
			return (*(result));
		else
		{
			if (std::holds_alternative<Object *>(std::get<Unit>(result->_content)) == false)
				return (*(result));
			else	
				return (*(result->as<Object*>()));
		}
	}

	const Object& operator [] (const std::wstring& p_key) const
	{
		Object* result = std::get<std::map<std::wstring, Object*>>(_content).at(p_key);

		if (std::holds_alternative<Unit>(result->_content) == false)
			return (*(result));
		else
		{
			if (std::holds_alternative<Object *>(std::get<Unit>(result->_content)) == false)
				return (*(result));
			else	
				return (*(result->as<Object*>()));
		}
	}

	Object& append()
	{
		if (_initialized == false)
		{
			_content = std::vector<Object*>();
			_initialized = true;
		}

		Object* result = new Object();
		std::get<std::vector<Object*>>(_content).push_back(result);
		return (*result);
	}

	void push_back(Object& p_object)
	{
		if (_initialized == false)
		{
			_content = std::vector<Object*>();
			_initialized = true;
		}
		Object* result = new Object(p_object);
		std::get<std::vector<Object*>>(_content).push_back(result);
	}

	Object& operator[](size_t p_index)
	{
		return (*(std::get<std::vector<Object*>>(_content)[p_index]));
	}

	const Object& operator[](size_t p_index) const
	{
		return (*(std::get<std::vector<Object*>>(_content)[p_index]));
	}

	size_t size() const 
	{
		return (std::get<std::vector<Object*>>(_content).size());
	}

	size_t count(const std::wstring& p_key) const
	{
		return (std::get<std::map<std::wstring, Object*>>(_content).count(p_key));
	}

	template<typename TType,
             typename std::enable_if<!std::is_same<TType, Object>::value, int>::type = 0>
	void set(const TType& p_value)
	{
		if (_initialized == false)
		{
			_content = Unit();
			_initialized = true;
		}

		std::get<Unit>(_content) = p_value;
	}

	template<typename TType,
			 typename std::enable_if<std::is_same<TType, Object>::value, int>::type = 0>
	void set(const TType& p_value)
	{
		Object* tmpObject = new Object(p_value);
		set<Object*>(tmpObject);
	}

	template<typename TType>
	const TType& as()
	{
		return (std::get<TType>(std::get<Unit>(_content)));
	}
};

int main()
{
	Object weapon = Object();
	weapon.addAttribute(L"Name").set<std::wstring>(L"Sword");
	weapon.addAttribute(L"Damage").set<int>(10);

	Object tmp;

	spk::cout << __LINE__ << std::endl;

	Object& objectA = tmp.append();
	Object& objectB = tmp.append();
	Object& objectC = tmp.append();

	objectA.set<int>(10);
	objectB.set<std::wstring>(L"Ceci est un test");

	objectC.addAttribute(L"Name").set<std::wstring>(L"NameOfTheObject"); 
	objectC.addAttribute(L"Level").set<int>(10);
	objectC.addAttribute(L"Weapon").push_back(weapon);
	objectC[L"Weapon"].push_back(weapon);

	spk::cout << "tmp A : " << std::endl;
	spk::cout << "tmp[0] = " << tmp[0].as<int>() << std::endl;
	spk::cout << "tmp[1] = " << tmp[1].as<std::wstring>() << std::endl;
	spk::cout << "tmp[2] = " << tmp[2][L"Name"].as<std::wstring>() << " / " << tmp[2][L"Level"].as<int>() << std::endl;
	spk::cout << "         and is equiped with " << tmp[2][L"Weapon"][0][L"Name"].as<std::wstring>() << std::endl;
	spk::cout << "                         and " << tmp[2][L"Weapon"][1][L"Name"].as<std::wstring>() << std::endl;

	return (0);
}