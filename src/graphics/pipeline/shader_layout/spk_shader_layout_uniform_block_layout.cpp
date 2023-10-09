#include "graphics/pipeline/spk_shader_layout.hpp"

#include <regex>

#include "spk_basic_functions.hpp"

namespace spk
{
	ShaderLayout::UniformBlock::UniformBlock(const StructureLayout &p_structureLayout) : FieldArrayLayout(p_structureLayout)
	{
	}

	void ShaderLayout::UniformBlock::_treatSingleUniform(const ShaderModule::Instruction &p_instruction)
	{
		spk::cout << "Parsing single unfirom [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
		// Regex for single uniform
		_mode = Mode::Single;
		std::regex singleUniformRegex("layout\\s*\\((?:set=(\\d+),\\s*)?binding=(\\d+)\\)\\s*uniform\\s+(\\w+)\\s+(\\w+);");
		std::smatch match;

		if (std::regex_search(p_instruction.code, match, singleUniformRegex))
		{
			int set = (match[1].length() > 0) ? std::stoi(match[1]) : -1;
			int binding = std::stoi(match[2]);
			_key = Key(set, binding);
			std::string dataType = match[3];
			_name = spk::to_wstring(match[4]);

			auto it = structureLayout.singleUniformStructures().find(dataType);
			if (it != structureLayout.singleUniformStructures().end())
			{
				Data pushConstantData = it->second;
				insert("", pushConstantData, -1);
			}
			else
			{
				spk::throwException(L"Unexpected SingleUniform type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]\nOnly accept the following types : " + structureLayout.acceptedSingleUniformTypeString());
			}
		}
		else
		{
			spk::throwException(L"Unexpected SingleUniform instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nlayout([OPTIONAL]set = [SetValue], binding = [BindingValue]) uniform [ValueType] [PushConstantName];");
		}
	}

	void ShaderLayout::UniformBlock::_treatUniformBlock(const ShaderModule::Instruction &p_instruction)
	{
		_mode = Mode::Block;
		std::regex blockUniformRegex("layout\\s*\\((?:set=(\\d+),\\s*)?binding=(\\d+)\\)\\s*uniform\\s+(\\w+)\\s*\\{([^\\}]*)\\}\\s+(\\w+);");
		std::smatch match;

		if (std::regex_search(p_instruction.code, match, blockUniformRegex))
		{
			int set = (match[1].length() > 0) ? std::stoi(match[1]) : 0;
			int binding = std::stoi(match[2]);
			_key = Key(set, binding);
			std::string type = match[3];
			std::string content = match[4];
			_name = spk::to_wstring(match[5]);

			std::regex fieldRegex("(\\w+)\\s+(\\w+);");

			// Search through content using the field regex
			auto words_begin = std::sregex_iterator(content.begin(), content.end(), fieldRegex);
			auto words_end = std::sregex_iterator();

			for (std::sregex_iterator i = words_begin; i != words_end; ++i)
			{
				std::smatch fieldMatch = *i;
				std::string dataType = fieldMatch[1];
				std::string variableName = fieldMatch[2];

				auto it = structureLayout.structures().find(dataType);
				if (it != structureLayout.structures().end())
				{
					Data pushConstantData = it->second;
					insert(variableName, pushConstantData, -1);
				}
				else
				{
					spk::throwException(L"Unexpected UniformBlock type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]");
				}
			}
		}
		else
		{
			spk::throwException(L"Unexpected UniformBlock instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nlayout([OPTIONAL]set = [SetValue], binding = [BindingValue]) uniform [PushConstantStructure] \n{\n    [ValueType] [ValueName];\n    [Repeat for each attribute];\n} [PushConstantName];");
		}
	}

	void ShaderLayout::UniformBlock::treat(const ShaderModule::Instruction &p_instruction)
	{
		if (p_instruction.type == ShaderModule::Instruction::Type::SingleUniform)
		{
			_treatSingleUniform(p_instruction);
		}
		else
		{
			_treatUniformBlock(p_instruction);
		}
	}

	ShaderLayout::UniformBlock::Key::Key(size_t p_set, size_t p_binding)
		: binding(p_binding), set(p_set)
	{
	}

	bool ShaderLayout::UniformBlock::Key::operator<(const ShaderLayout::UniformBlock::Key &p_other) const
	{
		if (binding < p_other.binding)
		{
			return true;
		}
		else if (binding == p_other.binding)
		{
			return set < p_other.set;
		}
		return false;
	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::UniformBlock::Mode& p_mode)
	{
		switch (p_mode)
		{
			case ShaderLayout::UniformBlock::Mode::Block:
				p_out << L"UniformBlock";
				break;
			case ShaderLayout::UniformBlock::Mode::Single:
				p_out << L"SamplerUniform";
				break;
			default:
				p_out << "Unexpected Mode";
				break;
		}
		return (p_out);
	}
	
	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::UniformBlock::Key& p_key)
	{
		p_out << L"Set : " << p_key.set << " / Binding : " << p_key.binding;
		return (p_out);
	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::UniformBlock& p_block)
	{
		p_out << "\t\t" << "Name : " << p_block._name << std::endl;
		p_out << "\t\t" << "Mode : " << p_block._mode << std::endl;
		p_out << "\t\t" << "Key : " << p_block._key << std::endl;
        p_out << static_cast<const ShaderLayout::FieldArrayLayout&>(p_block);
		return p_out;
	}

	const ShaderLayout::UniformBlock::Key &ShaderLayout::UniformBlock::key() const
	{
		return (_key);
	}

	const ShaderLayout::UniformBlock::Mode &ShaderLayout::UniformBlock::mode() const
	{
		return (_mode);
	}
}