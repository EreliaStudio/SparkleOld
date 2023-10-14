#pragma once

#include "data_structure/spk_data_buffer.hpp"
#include "graphics/pipeline/spk_shader_module.hpp"
#include "graphics/pipeline/spk_shader_layout.hpp"
#include <map>

#include "spk_basic_functions.hpp"

namespace spk
{
	class AbstractPipeline
	{
	protected:
		ShaderLayout _shaderLayout;

		virtual void _loadProgram(const ShaderLayout& p_shaderLayout) = 0;
		void _loadPipeline();

	public:
		AbstractPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);

		virtual void launch(const size_t& p_nbVertex) = 0;

		virtual void activate() = 0;
		virtual void deactivate() = 0;
	};
}