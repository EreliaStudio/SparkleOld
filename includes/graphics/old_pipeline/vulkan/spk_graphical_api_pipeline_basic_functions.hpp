#pragma once

#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

#include <vulkan/vulkan.hpp>
#include "glslang/SPIRV/GlslangToSpv.h"

namespace spk::GraphicalAPI
{

	struct SpirvHelper
	{
		static void init();
		static void finalize();

		static void initResources(TBuiltInResource& p_resources);

		static EShLanguage findLanguage(const vk::ShaderStageFlagBits shader_type);

		static bool GLSLtoSPV(const vk::ShaderStageFlagBits shader_type, const char* pshader, std::vector<unsigned int>& spirv);
	};

	vk::Format attributeFormat(const spk::GraphicalAPI::AbstractPipeline::Configuration::Data& p_data);
}