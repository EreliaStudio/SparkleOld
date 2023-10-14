#pragma once

#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	class Pipeline : public spk::AbstractPipeline
	{
	public:
		

	private:
		GLuint _program;
		
		void _loadProgram(const spk::ShaderLayout& p_shaderLayout);

	public:
		Pipeline(const spk::ShaderModule &p_vertexInput, const spk::ShaderModule &p_fragmentInput);

		void launch(const size_t& p_nbVertex);

		void activate();
		void deactivate();
	};
}