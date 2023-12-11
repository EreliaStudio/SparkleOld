#include "graphics/pipeline/spk_shader_layout.hpp"

#include <regex>

#include "spk_basic_functions.hpp"

namespace spk
{
	ShaderLayout::UniformBlockLayout::UniformBlockLayout(const StructureLayout &p_structureLayout) :
		FieldArrayLayout(p_structureLayout, 1 * sizeof(float))
	{
	}

	void ShaderLayout::UniformBlockLayout::_treatSamplerUniform(const ShaderModule::Instruction &p_instruction)
	{
		// Regex for single uniform
		_mode = Mode::Single;
		std::regex SamplerUniformRegex(R"(layout\s*\((?:set=(\d+),\s*)?binding=(\d+)\)\s*uniform\s+(\w+)\s+(\w+);)");
		std::smatch match;

		if (std::regex_search(p_instruction.code, match, SamplerUniformRegex))
		{
			int set = (match[1].length() > 0) ? std::stoi(match[1]) : -1;
			int binding = std::stoi(match[2]);
			_key = Key(set, binding);
			std::string dataType = match[3];
			_type = spk::to_wstring(dataType);
			_name = spk::to_wstring(match[4]);

			auto it = structureLayout.SamplerUniformStructures().find(dataType);
			if (it != structureLayout.SamplerUniformStructures().end())
			{
				Data pushConstantData = it->second;
				insert("", pushConstantData, -1);
			}
			else
			{
				spk::throwException(L"Unexpected SamplerUniform type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]\nOnly accept the following types : " + structureLayout.acceptedSamplerUniformTypeString());
			}
		}
		else
		{
			spk::throwException(L"Unexpected SamplerUniform instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nlayout([OPTIONAL]set = [SetValue], binding = [BindingValue]) uniform [ValueType] [PushConstantName];");
		}
	}

	void ShaderLayout::UniformBlockLayout::_treatUniformBlock(const ShaderModule::Instruction &p_instruction)
	{
		_mode = Mode::Block;
		std::regex blockUniformRegex(R"(layout\s*\((?:set=(\d+),\s*)?binding=(\d+)\)\s*uniform\s+(\w+)\s*\{([^\}]*)\}(?:\s*(\w+))?;)");
		std::smatch match;

		if (std::regex_search(p_instruction.code, match, blockUniformRegex))
		{
			int set = (match[1].length() > 0) ? std::stoi(match[1]) : 0;
			int binding = std::stoi(match[2]);
			_key = Key(set, binding);
			std::string type = match[3];
			std::string content = match[4];
			_type = spk::to_wstring(type);
			_name = spk::to_wstring(match[5]);

			std::regex fieldRegex(R"((\w+)\s+(\w+);)");

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
					spk::throwException(L"Unexpected UniformBlockLayout type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]");
				}
			}
		}
		else
		{
			spk::throwException(L"Unexpected UniformBlockLayout instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nlayout([OPTIONAL]set = [SetValue], binding = [BindingValue]) uniform [UniformBlockStructure] \n{\n    [ValueType] [ValueName];\n    [Repeat for each attribute];\n} [[OPTIONAL]UniformBlockName];");
		}
	}

	void ShaderLayout::UniformBlockLayout::treat(const ShaderModule::Instruction &p_instruction)
	{
		if (p_instruction.type == ShaderModule::Instruction::Type::SamplerUniform)
		{
			_treatSamplerUniform(p_instruction);
		}
		else
		{
			_treatUniformBlock(p_instruction);
		}
	}

	ShaderLayout::UniformBlockLayout::Key::Key(size_t p_set, size_t p_binding)
		: binding(p_binding), set(p_set)
	{
	}

	bool ShaderLayout::UniformBlockLayout::Key::operator<(const ShaderLayout::UniformBlockLayout::Key &p_other) const
	{
		if (binding < p_other.binding)
		{
			return (true);
		}
		else if (binding == p_other.binding)
		{
			return (set < p_other.set);
		}
		return (false);
	}
	
	bool ShaderLayout::UniformBlockLayout::Key::operator==(const ShaderLayout::UniformBlockLayout::Key &p_other) const
	{
		return (binding == p_other.binding && set == p_other.set);
	}

	bool ShaderLayout::UniformBlockLayout::Key::operator!=(const ShaderLayout::UniformBlockLayout::Key &p_other) const
	{
		return (binding != p_other.binding || set != p_other.set);
	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::UniformBlockLayout::Mode& p_mode)
	{
		switch (p_mode)
		{
			case ShaderLayout::UniformBlockLayout::Mode::Block:
				p_out << L"UniformBlockLayout";
				break;
			case ShaderLayout::UniformBlockLayout::Mode::Single:
				p_out << L"SamplerUniform";
				break;
			default:
				p_out << "Unexpected Mode";
				break;
		}
		return (p_out);
	}
	
	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::UniformBlockLayout::Key& p_key)
	{
		p_out << L"Set : " << p_key.set << " / Binding : " << p_key.binding;
		return (p_out);
	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::UniformBlockLayout& p_block)
	{
		p_out << "\t\t" << "Name : " << p_block._name << std::endl;
		p_out << "\t\t" << "Mode : " << p_block._mode << std::endl;
		p_out << "\t\t" << "Key : " << p_block._key << std::endl;
        p_out << static_cast<const ShaderLayout::FieldArrayLayout&>(p_block);
		return p_out;
	}

	const std::wstring &ShaderLayout::UniformBlockLayout::name() const
	{
		return (_name);
	}

	const std::wstring &ShaderLayout::UniformBlockLayout::type() const
	{
		return (_type);
	}

	const ShaderLayout::UniformBlockLayout::Key &ShaderLayout::UniformBlockLayout::key() const
	{
		return (_key);
	}

	const ShaderLayout::UniformBlockLayout::Mode &ShaderLayout::UniformBlockLayout::mode() const
	{
		return (_mode);
	}
}
