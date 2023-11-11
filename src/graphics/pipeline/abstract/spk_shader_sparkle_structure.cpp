#include "graphics/pipeline/spk_shader_module.hpp"

#include "spk_basic_functions.hpp"

#include <regex>

namespace spk
{

	bool ShaderModule::_checkStructureNameCollision()
	{
		std::smatch matches;
		for (const auto& structure : structuresArray)
		{
			std::regex firstRegex(R"(^(.*?)\{)");
			if (std::regex_search(structure, matches, firstRegex))
			{
				std::string structureName = matches[1].str();
				std::regex collisionRegex(structureName);
				if (std::regex_search(_code, collisionRegex))
				{
					return true;
				}
			}
		}
		return false;
	}

	size_t ShaderModule::_findInsertionPoint()
	{
		return (_code.find('\n') + 1);
	}

	void ShaderModule::_insertStructuresLines()
	{
		size_t nbStructInserted = 0;
		size_t insertionPoint = _findInsertionPoint();

		for (const auto& structure : structuresArray)
		{
			_code.insert(insertionPoint, "\n" + structure + "\n");
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