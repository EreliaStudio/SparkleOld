#pragma once

#include <variant>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

#include "spk_basic_functions.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
    namespace JSON
    {

        class Unit
        {
        private:
            std::variant<bool, int, double, std::wstring> _data;

        public:
            Unit() : _data(false) {}

            template <typename TType>
            void set(const TType &p_type)
            {
                _data = p_type;
            }

            template <typename TType>
            TType as() const
            {
                return std::get<TType>(_data);
            }
        };

        class Object
        {
        public:
            using Attribute = std::variant<Unit, std::vector<Unit>, Object, std::vector<Object>>;

        private:
            std::map<std::wstring, Attribute> _attributes;

        public:
            Object()
            {
                
            }

            void addAttribute(const std::wstring &p_name, const Unit &p_value)
            {
                _attributes[p_name] = p_value;
            }

            void addAttribute(const std::wstring &p_name, const std::vector<Unit> &p_values)
            {
                _attributes[p_name] = p_values;
            }

            void addAttribute(const std::wstring &p_name, const Object &p_object)
            {
                _attributes[p_name] = p_object;
            }

            void addAttribute(const std::wstring &p_name, const std::vector<Object> &p_objects)
            {
                _attributes[p_name] = p_objects;
            }

            void addAttribute(const std::wstring &p_name)
            {
                _attributes[p_name] = Unit();
            }

            template <typename TType>
            void addAttribute(const std::wstring &p_name, const TType& p_value)
            {
                _attributes[p_name] = Unit();
                std::get<Unit>(_attributes[p_name]).set<TType>(p_value);
            }

            void addAttributeArray(const std::wstring &p_name)
            {
                _attributes[p_name] = std::vector<Unit>();
            }

            void addObject(const std::wstring &p_name)
            {
                _attributes[p_name] = Object();
            }

            void addObjectArray(const std::wstring &p_name)
            {
                _attributes[p_name] = std::vector<Object>();
            }

            Unit &get(const std::wstring &p_name)
            {
                if (_attributes.count(p_name) == 0)
                    spk::throwException(L"Can't access to attribute [" + p_name + L"] inside JSON::Object");
                return (std::get<Unit>(_attributes.at(p_name)));
            }

            std::vector<Unit> &getArray(const std::wstring &p_name)
            {
                if (_attributes.count(p_name) == 0)
                    spk::throwException(L"Can't access to attribute array [" + p_name + L"] inside JSON::Object");
                return (std::get<std::vector<Unit>>(_attributes.at(p_name)));
            }

            Object &getObject(const std::wstring &p_name)
            {
                if (_attributes.count(p_name) == 0)
                    spk::throwException(L"Can't access to object [" + p_name + L"] inside JSON::Object");
                return (std::get<Object>(_attributes.at(p_name)));
            }

            std::vector<Object> &getObjectArray(const std::wstring &p_name)
            {
                if (_attributes.count(p_name) == 0)
                    spk::throwException(L"Can't access to object array [" + p_name + L"] inside JSON::Object");
                return (std::get<std::vector<Object>>(_attributes.at(p_name)));
            }

            const Unit &get(const std::wstring &p_name) const
            {
                if (_attributes.count(p_name) == 0)
                    spk::throwException(L"Can't access to attribute [" + p_name + L"] inside JSON::Object");
                return (std::get<Unit>(_attributes.at(p_name)));
            }

            const std::vector<Unit> &getArray(const std::wstring &p_name) const
            {
                if (_attributes.count(p_name) == 0)
                    spk::throwException(L"Can't access to attribute array [" + p_name + L"] inside JSON::Object");
                return (std::get<std::vector<Unit>>(_attributes.at(p_name)));
            }

            const Object &getObject(const std::wstring &p_name) const
            {
                if (_attributes.count(p_name) == 0)
                    spk::throwException(L"Can't access to object [" + p_name + L"] inside JSON::Object");
                return (std::get<Object>(_attributes.at(p_name)));
            }

            const std::vector<Object> &getObjectArray(const std::wstring &p_name) const
            {
                if (_attributes.count(p_name) == 0)
                    spk::throwException(L"Can't access to object array [" + p_name + L"] inside JSON::Object");
                return (std::get<std::vector<Object>>(_attributes.at(p_name)));
            }
        };

        class File
        {
        private:
        public:
            File() = default;

            File(const std::wstring &p_filePath)
            {
                load(p_filePath);
            }

            void load(const std::wstring &p_filePath)
            {
                std::wfstream file;

                file.open(p_filePath.c_str(), std::ios_base::in);

                if (file.is_open() == false)
                {
                    throwException(L"Can open file [" + p_filePath + L"]");
                }

                file.close();
            }
        };
    }
}