#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	std::shared_ptr<AbstractPipeline::Uniform> AbstractPipeline::_loadUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		switch (p_uniformBlockLayout.mode())
		{
		case ShaderLayout::UniformBlockLayout::Mode::Block:
			return (_loadUniformBlock(p_uniformBlockLayout));
		case ShaderLayout::UniformBlockLayout::Mode::Single:
			return (_loadSamplerUniform(p_uniformBlockLayout));
		}
		return (nullptr);
	}

	void AbstractPipeline::_loadPipeline()
	{
		_loadProgram(_shaderLayout);

		for (const auto& uniformBlockLayout : _shaderLayout.uniformBlockLayouts())
		{
			if (_uniformMap.contains(uniformBlockLayout.key()) == false)
			{
				std::shared_ptr<Uniform> newUniform = _loadUniform(uniformBlockLayout);

				_uniformMap[uniformBlockLayout.key()] = newUniform;
			}

			std::shared_ptr<Uniform> targetUniform = _uniformMap[uniformBlockLayout.key()];

			if (targetUniform->size() != uniformBlockLayout.stride())
				spk::throwException(L"Uniform named [" + uniformBlockLayout.name() + L"] collide with an existing uniform of different size, at set [" + std::to_wstring(uniformBlockLayout.key().set) + L"] and binding [" + std::to_wstring(uniformBlockLayout.key().binding) + L"]");
			
			_uniformKeys[uniformBlockLayout.name()] = uniformBlockLayout.key();
		}
	}

	AbstractPipeline::AbstractPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) : _shaderLayout(p_vertexInput, p_fragmentInput)
	{

	}

	AbstractPipeline::Uniform& AbstractPipeline::uniform(const std::wstring& p_uniformName)
	{
		if (_uniformKeys.contains(p_uniformName) == false)
			spk::throwException(L"Uniform named [" + p_uniformName + L"] doesn't exist.");
		ShaderLayout::UniformBlockLayout::Key uniformKey = _uniformKeys[p_uniformName];

		if (_uniformMap.contains(uniformKey) == false)
			spk::throwException(L"Uniform named [" + p_uniformName + L"] doesn't link to an existing key [" + std::to_wstring(uniformKey.set) + L" / " + std::to_wstring(uniformKey.binding) + L"].");
	
		return (*(_uniformMap[uniformKey]));
	}

	std::shared_ptr<AbstractPipeline::Object> AbstractPipeline::createObject()
	{
		return (_loadObject(_shaderLayout.storageBufferLayout(), _shaderLayout.pushConstantsLayout()));
	}
}