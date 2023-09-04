#include "graphics/spk_abstract_graphical_device.hpp"

namespace spk
{
	AbstractGraphicalDevice::AbstractGraphicalDevice()
	{
		
	}

		virtual void _loadDevice(
			const std::string &p_vertexModuleName, const std::string &p_vertexModuleCode,
			const std::string &p_fragmentModuleName, const std::string &p_fragmentModuleCode) = 0;

	void AbstractGraphicalDevice::load(const std::wstring& p_vertexShaderCode, const std::wstring& p_fragmentShaderCode)
	{
		_loadDevice("VertexCode", spk::wstringToString(p_vertexShaderCode), "FragmentCode", spk::wstringToString(p_fragmentShaderCode));
	}

	void AbstractGraphicalDevice::loadFromFile(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath)
	{
		_loadDevice(
				p_vertexShaderPath.filename().string(), getFileContentAsString(p_vertexShaderCode),
				p_fragmentShaderPath.filename().string(), getFileContentAsString(p_fragmentShaderCode)
			);
	}
}