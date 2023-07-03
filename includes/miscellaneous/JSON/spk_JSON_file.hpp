#pragma once

#include "miscellaneous/JSON/spk_JSON_object.hpp"
#include <filesystem>

namespace spk
{
	namespace JSON
	{
		class File
		{
		private:
			spk::JSON::Object _root;

			static const int _ERROR_TYPE = -1;
			static const int _UNIT_TYPE = 0;
			static const int _OBJECT_TYPE = 1;
			static const int _ARRAY_TYPE = 2;
			static const int _ERROR_UNIT_TYPE = -1;
			static const int _STRING_UNIT_TYPE = 0;
			static const int _INT_UNIT_TYPE = 1;
			static const int _DOUBLE_UNIT_TYPE = 2;
			static const int _BOOL_UNIT_TYPE = 3;
			static const int _NULL_UNIT_TYPE = 4;

			void _removeUnnecessaryChar(std::wstring &p_fileContent);
			std::wstring _loadFileContent(const std::filesystem::path &p_filePath);

			std::wstring _removeEscapingChar(const std::wstring &p_string);
			std::wstring _getAttributeName(const std::wstring &p_content, size_t &p_index);

			int _getLoadingType(const std::wstring &p_content, size_t &p_index);
			std::wstring _extractUnitSubstring(const std::wstring &p_content, size_t &p_index);


			int _getUnitType(const std::wstring &p_unitSubString);

			void _loadUnitString(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);
			void _loadUnitInt(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);
			void _loadUnitDouble(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);
			void _loadUnitBoolean(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);
			void _loadUnitNull(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);
			void _loadUnit(spk::JSON::Object &p_objectToFill, const std::wstring &p_content, size_t &p_index);
			void _loadObject(spk::JSON::Object &p_objectToFill, const std::wstring &p_content, size_t &p_index);
			void _loadArray(spk::JSON::Object &p_objectToFill, const std::wstring &p_content, size_t &p_index);
			void _loadContent(spk::JSON::Object &p_objectToFill, const std::wstring &p_content, size_t &p_index);

		public:
			File();
			File(const std::filesystem::path& p_filePath);

			void load(const std::filesystem::path& p_filePath);

			const spk::JSON::Object &operator[](const std::wstring &p_key) const;
			const spk::JSON::Object &operator[](size_t p_index) const;

			template <typename TType>
			const TType &as()
			{
				return (_root.as<TType>());
			}

			const spk::JSON::Object &root() const;

			friend std::wostream &operator<<(std::wostream &p_os, const File &p_file);
		};
	}
}