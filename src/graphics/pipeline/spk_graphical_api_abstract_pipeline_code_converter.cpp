#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	extern std::string predefinedStructures;

	void insertPredefinedStructures(std::string &p_shaderCode)
	{

		std::size_t versionEndPos = p_shaderCode.find("\n");
		if (versionEndPos != std::string::npos)
		{
			p_shaderCode.insert(versionEndPos + 1, predefinedStructures);
		}
	}

	void replacePushConstantsWithUniforms(std::string &p_shaderCode)
    {
        std::regex pushConstantRegex("layout\\(push_constant\\) uniform ([\\w_]+)\\s*\\{([\\s\\S]*?)\\}\\s*([\\w_]+);");
        std::smatch match;

        while (std::regex_search(p_shaderCode, match, pushConstantRegex))
        {
            std::string structName = match[1].str();
            std::string content = match[2].str();
            std::string uniformName = match[3].str();

            std::string replacement = "struct " + structName + " {\n" + content + "\n};\nuniform " + structName + " " + uniformName + ";";

            p_shaderCode.replace(match.position(), match.length(), replacement);
        }
    }

	void convertVulkanToOpenglCompatibleGLSLCode(std::string &p_vertexCode, std::string &p_fragmentCode)
	{
		insertPredefinedStructures(p_vertexCode);
	    insertPredefinedStructures(p_fragmentCode);

		replacePushConstantsWithUniforms(p_vertexCode);
        replacePushConstantsWithUniforms(p_fragmentCode);
	}
}