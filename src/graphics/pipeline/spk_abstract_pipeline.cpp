#include "graphics/pipeline/spk_abstract_pipeline.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	void AbstractPipeline::_loadPipeline()
	{
		_loadProgram(_shaderLayout);

		for (const auto& uniformBlockLayout : _shaderLayout.uniformBlockLayouts())
		{
			addUniformBlock(uniformBlockLayout);
		}
	}

	AbstractPipeline::AbstractPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) :
		_shaderLayout(p_vertexInput, p_fragmentInput)
	{

	}

	std::shared_ptr<AbstractPipeline::Object> AbstractPipeline::createObject()
	{
		return (_loadObject(_shaderLayout.storageBufferLayout(), _shaderLayout.pushConstantLayout()));
	}

	void AbstractPipeline::addUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		if (_uniformBlocksKeys.contains(p_uniformBlockLayout.name()) == true && _uniformBlocksKeys[p_uniformBlockLayout.name()] != p_uniformBlockLayout.key())
			spk::throwException(L"UniformBlock [" + p_uniformBlockLayout.name() + L"] exist already with a different key");

		_uniformBlocksKeys[p_uniformBlockLayout.name()] = p_uniformBlockLayout.key();

		if (_uniformBlocks.contains(p_uniformBlockLayout.key()) == false)
		{
			if (p_uniformBlockLayout.mode() == ShaderLayout::UniformBlockLayout::Mode::Block)
				_uniformBlocks[p_uniformBlockLayout.key()] = _loadUniformBlock(p_uniformBlockLayout);
			else
				_uniformBlocks[p_uniformBlockLayout.key()] = _loadSamplerUniform(p_uniformBlockLayout);
		}
	}

	std::shared_ptr<AbstractPipeline::Uniform> AbstractPipeline::getUniform(const std::wstring& p_uniformBlockName)
	{
		if (_uniformBlocksKeys.contains(p_uniformBlockName) == false)
			spk::throwException(L"Uniform block named [" + p_uniformBlockName + L"] doesn't exist");

		spk::ShaderLayout::UniformBlockLayout::Key key = _uniformBlocksKeys[p_uniformBlockName];
		
		if (_uniformBlocks.contains(key) == true)
			spk::throwException(L"Key corresponding to [" + p_uniformBlockName + L"] uniform block doesn't exist");

		return (_uniformBlocks[key]);
	}
}