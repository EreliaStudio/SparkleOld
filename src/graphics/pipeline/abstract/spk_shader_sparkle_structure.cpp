#include "graphics/pipeline/spk_shader_module.hpp"

#include "spk_basic_functions.hpp"

#include <regex>
#include <map>

namespace spk
{
	std::map<std::string, std::string> structuresMap = {
			// {"StructA", "struct StructA{vec4 color;};"},
			// {"StructB", "struct StructB{float ratio; vec2 position;};"}
		};

	bool ShaderModule::_checkStructureNameCollision()
	{
		for (const auto& structure : structuresMap)
		{
			std::regex collisionRegex("struct\\s+" + structure.first + "\\s*;");
			if (std::regex_search(_code, collisionRegex)) {
				return (true);
			}
		}
		return (false);
	}

	size_t ShaderModule::_findInsertionPoint()
	{
		return (_code.find('\n') + 1);
	}

	void ShaderModule::_insertStructuresLines()
	{
		size_t nbStructInserted = 0;
		size_t insertionPoint = _findInsertionPoint();

		for (const auto& structure : structuresMap)
		{
			_code.insert(insertionPoint, "\n" + structure.second + "\n");
		}
	}

	void ShaderModule::_insertSparkleStructuresDefinition()
	{
		if (_checkStructureNameCollision() == true)
		{
			spk::throwException(L"Can't insert Sparkle structure : client used a name already used by Sparkle");
		}

		_insertStructuresLines();
	}
}