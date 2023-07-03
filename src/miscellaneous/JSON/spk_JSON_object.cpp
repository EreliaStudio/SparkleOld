#include "miscellaneous/JSON/spk_JSON_object.hpp"

#include "spk_basic_functions.hpp"

namespace spk
{
	namespace JSON
	{
			Object::Object()
			{
				_initialized = false;
			}

			void Object::reset()
			{
				_initialized = false;
				_content = ContentType();
			}

			Object &Object::addAttribute(const std::wstring &p_key)
			{
				if (_initialized == false)
				{
					_content = std::map<std::wstring, Object *>();
					_initialized = true;
				}

				Object *result = new Object();
				std::get<std::map<std::wstring, Object *>>(_content)[p_key] = result;
				return (*result);
			}

			/*
				Forced to do this to prevent user from writing as<Object*>() after getting an object from the map
				it's not mandatory, but it's easier to read for the user with this.

				If not keeped, we coult simply :
				return (std::get<std::map<std::wstring, Object*>>(_content).at(p_key));
			*/
			Object &Object::operator[](const std::wstring &p_key)
			{
				auto map = std::get<std::map<std::wstring, Object *>>(_content);

				if (map.count(p_key) == 0)
					spk::throwException(L"Can't acces JSON object named [" + p_key + L"] : it does not exist");

				Object *result = map.at(p_key);

				if (std::holds_alternative<Unit>(result->_content) == false)
					return (*(result));
				else
				{
					if (std::holds_alternative<Object *>(std::get<Unit>(result->_content)) == false)
						return (*(result));
					else
						return (*(result->as<Object *>()));
				}
			}

			const Object &Object::operator[](const std::wstring &p_key) const
			{
				auto map = std::get<std::map<std::wstring, Object *>>(_content);

				if (map.count(p_key) == 0)
					spk::throwException(L"Can't acces JSON object named [" + p_key + L"] : it does not exist");

				Object *result = map.at(p_key);

				if (std::holds_alternative<Unit>(result->_content) == false)
					return (*(result));
				else
				{
					if (std::holds_alternative<Object *>(std::get<Unit>(result->_content)) == false)
						return (*(result));
					else
						return (*(result->as<Object *>()));
				}
			}

			Object &Object::append()
			{
				if (_initialized == false)
				{
					_content = std::vector<Object *>();
					_initialized = true;
				}

				Object *result = new Object();
				std::get<std::vector<Object *>>(_content).push_back(result);
				return (*result);
			}

			void Object::push_back(Object &p_object)
			{
				if (_initialized == false)
				{
					_content = std::vector<Object *>();
					_initialized = true;
				}
				Object *result = new Object(p_object);
				std::get<std::vector<Object *>>(_content).push_back(result);
			}

			Object &Object::operator[](size_t p_index)
			{
				return (*(std::get<std::vector<Object *>>(_content)[p_index]));
			}

			const Object &Object::operator[](size_t p_index) const
			{
				return (*(std::get<std::vector<Object *>>(_content)[p_index]));
			}

			size_t Object::size() const
			{
				return (std::get<std::vector<Object *>>(_content).size());
			}

			size_t Object::count(const std::wstring &p_key) const
			{
				return (std::get<std::map<std::wstring, Object *>>(_content).count(p_key));
			}

			void Object::printUnit(std::wostream &p_os) const
			{
				const Unit &tmp = std::get<Unit>(_content);

				switch (tmp.index())
				{
				case 0:
					p_os << std::boolalpha << as<bool>();
					break;
				case 1:
					p_os << as<int>();
					break;
				case 2:
					p_os << as<double>();
					break;
				case 3:
					p_os << as<std::wstring>();
					break;
				case 4:
					p_os << *(as<Object *>());
					break;
				case 5:
					p_os << L"Null";
					break;
				}
			}

			void Object::printObject(std::wostream &p_os) const
			{
				const std::map<std::wstring, Object *> &map = std::get<std::map<std::wstring, Object *>>(_content);

				p_os << L"{" << std::endl;
				for (auto &tmp : map)
				{
					p_os << tmp.first << " : " << *(tmp.second) << std::endl;
				}
				p_os << L"}";
			}

			void Object::printArray(std::wostream &p_os) const
			{
				const std::vector<Object *> &vector = std::get<std::vector<Object *>>(_content);

				p_os << L"[" << std::endl;
				for (size_t i = 0; i < vector.size(); i++)
				{
					p_os << L"Element [" << i << L"] : " << *(vector[i]) << std::endl;
				}
				p_os << L"]";
			}

			std::wostream &operator<<(std::wostream &p_os, const Object &p_object)
			{
				switch (p_object._content.index())
				{
				case 0:
					p_object.printUnit(p_os);
					break;
				case 1:
					p_object.printObject(p_os);
					break;
				case 2:
					p_object.printArray(p_os);
					break;
				}
				return (p_os);
			}
	}
}