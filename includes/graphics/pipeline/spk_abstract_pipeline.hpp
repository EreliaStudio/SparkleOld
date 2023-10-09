#pragma once

#include "graphics/pipeline/spk_shader_module.hpp"
#include "graphics/pipeline/spk_shader_layout.hpp"
#include <map>

namespace spk
{
	class AbstractPipeline
	{
	protected:
		virtual void _loadProgram(const ShaderLayout &p_configuration, const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) = 0;

		void _loadPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);

	public:
	};
}