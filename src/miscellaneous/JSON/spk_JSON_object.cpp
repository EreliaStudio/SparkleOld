#include "miscellaneous/JSON/spk_JSON_object.hpp"

#include "spk_basic_functions.hpp"

namespace spk
{
	namespace JSON
	{
		size_t Object::_indent = 0;

		Object::Object(const std::wstring& p_name) :
			_name(p_name)
		{
			_initialized = false;
		}

		void Object::reset()
		{
			_initialized = false;
			_content = ContentType();
		}

		bool Object::isObject() const
		{
			return (std::holds_alternative<std::map<std::wstring, Object*>>(_content));
		}

		bool Object::isArray() const
		{
			return (std::holds_alternative<std::vector<Object*>>(_content));
		}

		bool Object::isUnit() const
		{
			return (std::holds_alternative<Unit>(_content));
		}

		Object& Object::addAttribute(const std::wstring& p_key)
		{
			if (_initialized == false)
			{
				_content = std::map<std::wstring, Object*>();
				_initialized = true;
			}

			Object* result = new Object(p_key);

			if (std::get<std::map<std::wstring, Object*>>(_content).count(p_key) != 0)
				spk::throwException(L"Can't add attribute named [" + p_key + L"] : it already exists");
			std::get<std::map<std::wstring, Object*>>(_content)[p_key] = result;
			return (*result);
		}
		
		bool Object::contains(const std::wstring& p_key) const
		{
			auto& map = std::get<std::map<std::wstring, Object*>>(_content);

			return (map.contains(p_key));
		}

		/*
			Forced to do this to prevent user from writing as<Object*>() after getting an object from the map
			it's not mandatory, but it's easier to read for the user with this.

			If not keeped, we coult simply :
			return (std::get<std::map<std::wstring, Object*>>(_content).at(p_key));
		*/
		Object& Object::operator[](const std::wstring& p_key)
		{
			auto& map = std::get<std::map<std::wstring, Object*>>(_content);

			if (map.count(p_key) == 0)
				spk::throwException(L"Can't acces JSON object named [" + p_key + L"] : it does not exist");

			Object* result = map.at(p_key);

			if (std::holds_alternative<Unit>(result->_content) == false)
				return (*(result));
			else
			{
				if (std::holds_alternative<Object*>(std::get<Unit>(result->_content)) == false)
					return (*(result));
				else
					return (*(result->as<Object*>()));
			}
		}

		const Object& Object::operator[](const std::wstring& p_key) const
		{
			auto& map = std::get<std::map<std::wstring, Object*>>(_content);

			if (map.count(p_key) == 0)
				spk::throwException(L"Can't acces JSON object named [" + p_key + L"] : it does not exist");

			Object* result = map.at(p_key);

			if (std::holds_alternative<Unit>(result->_content) == false)
				return (*(result));
			else
			{
				if (std::holds_alternative<Object*>(std::get<Unit>(result->_content)) == false)
					return (*(result));
				else
					return (*(result->as<Object*>()));
			}
		}

		const std::map<std::wstring, Object*>& Object::members() const
		{
			if (_initialized == false || std::holds_alternative<std::map<std::wstring, Object*>>(_content) == false)
				spk::throwException(L"Can't get object members : object is not initialized or is not of type object");
			return (std::get<std::map<std::wstring, Object*>>(_content));
		}

		void	Object::setAsObject()
		{
			if (_initialized == true)
				spk::throwException(L"Can't set object as object : it is already initialized");
			_content = std::map<std::wstring, Object*>();
			_initialized = true;
		}
		
		void Object::resize(size_t p_size)
		{
			if (_initialized == false)
			{
				_content = std::vector<Object*>();
				_initialized = true;
			}
			std::vector<Object*>& array = std::get<std::vector<Object*>>(_content);

			for (size_t i = p_size; i < array.size(); i++)
				delete array[i];

			for (size_t i = array.size(); i < p_size; i++)
				array.push_back(new Object(L"[" + std::to_wstring(i) + L"]"));
		}

		Object& Object::append()
		{
			if (_initialized == false)
			{
				_content = std::vector<Object*>();
				_initialized = true;
			}

			Object* result = new Object(L"[" + std::to_wstring(std::get<std::vector<Object*>>(_content).size()) + L"]");
			std::get<std::vector<Object*>>(_content).push_back(result);
			return (*result);
		}

		void Object::push_back(Object& p_object)
		{
			if (_initialized == false)
			{
				_content = std::vector<Object*>();
				_initialized = true;
			}
			Object* result = new Object(p_object);
			std::get<std::vector<Object*>>(_content).push_back(result);
		}

		Object& Object::operator[](size_t p_index)
		{
			return (*(std::get<std::vector<Object*>>(_content)[p_index]));
		}

		const Object& Object::operator[](size_t p_index) const
		{
			return (*(std::get<std::vector<Object*>>(_content)[p_index]));
		}

		void	Object::setAsArray()
		{
			if (_initialized == true)
				spk::throwException(L"Can't set object as Array : it is already initialized");
			_content = std::vector<Object*>();
			_initialized = true;
		}

		size_t Object::size() const
		{
			return (std::get<std::vector<Object*>>(_content).size());
		}

		size_t Object::count(const std::wstring& p_key) const
		{
			return (std::get<std::map<std::wstring, Object*>>(_content).count(p_key));
		}

		static std::wstring _cleanUnicodeString(const std::wstring& p_string)
		{
			std::wstring result(p_string);

			for (size_t i(0); i < result.size(); ++i)
			{
				const wchar_t c = result[i];
				std::wstring universalCodedCharSet;

				switch (c)
				{
				case L'\\':
					result.replace(i, 1, L"\\u005c");
					i += 5;
					break;
				case L'\"':
					result.replace(i, 1, L"\\\"");
					i += 1;
					break;
				case L'\b':
					result.replace(i, 1, L"\\b");
					i += 1;
					break;
				case L'\f':
					result.replace(i, 1, L"\\f");
					i += 1;
					break;
				case L'\n':
					result.replace(i, 1, L"\\n");
					i += 1;
					break;
				case L'\r':
					result.replace(i, 1, L"\\r");
					i += 1;
					break;
				case L'\t':
					result.replace(i, 1, L"\\t");
					i += 1;
					break;
				default:
					if (c < 0x20 || c > 0x7E)
					{
						universalCodedCharSet = spk::universalCodeDecoder(c);

						result.replace(i, 1, universalCodedCharSet);
						i += universalCodedCharSet.size() - 1;
					}
					break;
				}
			}
			return (result);
		}

		void Object::printUnit(std::wostream& p_os) const
		{
			const Unit& tmp = std::get<Unit>(_content);

			switch (tmp.index())
			{
			case 0:
				p_os << std::boolalpha << as<bool>();
				break;
			case 1:
				p_os << as<long>();
				break;
			case 2:
				p_os << as<double>();
				break;
			case 3:
				p_os << L'"' << _cleanUnicodeString(as<std::wstring>()) << L'"';
				break;
			case 4:
				p_os << *(as<Object*>());
				break;
			case 5:
				p_os << L"null";
				break;
			}
		}

		void Object::printObject(std::wostream& p_os) const
		{
			const std::map<std::wstring, Object*>& map = std::get<std::map<std::wstring, Object*>>(_content);
			std::wstring cleanedKey;

			p_os << std::setw(_indent * _indentSize) << L"" << L"{" << std::endl;
			++_indent;
			for (auto& tmp : map)
			{
				cleanedKey = _cleanUnicodeString(tmp.first);

				p_os << std::setw(_indent * _indentSize) << L"" <<
					L'"' << cleanedKey << L"\": ";

				if (tmp.second->_content.index() == 1 ||
					tmp.second->_content.index() == 2)
					p_os << std::endl;

				p_os << *(tmp.second);
				if (&tmp != &(*map.rbegin()))
					p_os << L',';
				p_os << std::endl;
			}
			--_indent;
			p_os << std::setw(_indent * _indentSize) << L"" << L"}";
		}

		void Object::printArray(std::wostream& p_os) const
		{
			const std::vector<Object*>& vector = std::get<std::vector<Object*>>(_content);

			p_os << std::setw(_indent * _indentSize) << L"" << L'[' << std::endl;
			++_indent;
			for (size_t i = 0; i < vector.size(); i++)
			{
				p_os << std::setw(_indent * _indentSize) << L"" << *(vector[i]);
				if (i != vector.size() - 1)
					p_os << L',';
				p_os << std::endl;
			}
			--_indent;
			p_os << std::setw(_indent * _indentSize) << L"" << L']';
		}

		std::wostream& operator<<(std::wostream& p_os, const Object& p_object)
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