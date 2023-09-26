#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline_basic_functions.hpp"

#include <glslang/SPIRV/GlslangToSpv.h>

namespace spk::GraphicalAPI
{
	void SpirvHelper::init()
	{
		glslang::InitializeProcess();
	}

	void SpirvHelper::finalize()
	{
		glslang::FinalizeProcess();
	}

	void SpirvHelper::initResources(TBuiltInResource& p_resources)
	{
		p_resources.maxLights = 32;
		p_resources.maxClipPlanes = 6;
		p_resources.maxTextureUnits = 32;
		p_resources.maxTextureCoords = 32;
		p_resources.maxVertexAttribs = 64;
		p_resources.maxVertexUniformComponents = 4096;
		p_resources.maxVaryingFloats = 64;
		p_resources.maxVertexTextureImageUnits = 32;
		p_resources.maxCombinedTextureImageUnits = 80;
		p_resources.maxTextureImageUnits = 32;
		p_resources.maxFragmentUniformComponents = 4096;
		p_resources.maxDrawBuffers = 32;
		p_resources.maxVertexUniformVectors = 128;
		p_resources.maxVaryingVectors = 8;
		p_resources.maxFragmentUniformVectors = 16;
		p_resources.maxVertexOutputVectors = 16;
		p_resources.maxFragmentInputVectors = 15;
		p_resources.minProgramTexelOffset = -8;
		p_resources.maxProgramTexelOffset = 7;
		p_resources.maxClipDistances = 8;
		p_resources.maxComputeWorkGroupCountX = 65535;
		p_resources.maxComputeWorkGroupCountY = 65535;
		p_resources.maxComputeWorkGroupCountZ = 65535;
		p_resources.maxComputeWorkGroupSizeX = 1024;
		p_resources.maxComputeWorkGroupSizeY = 1024;
		p_resources.maxComputeWorkGroupSizeZ = 64;
		p_resources.maxComputeUniformComponents = 1024;
		p_resources.maxComputeTextureImageUnits = 16;
		p_resources.maxComputeImageUniforms = 8;
		p_resources.maxComputeAtomicCounters = 8;
		p_resources.maxComputeAtomicCounterBuffers = 1;
		p_resources.maxVaryingComponents = 60;
		p_resources.maxVertexOutputComponents = 64;
		p_resources.maxGeometryInputComponents = 64;
		p_resources.maxGeometryOutputComponents = 128;
		p_resources.maxFragmentInputComponents = 128;
		p_resources.maxImageUnits = 8;
		p_resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		p_resources.maxCombinedShaderOutputResources = 8;
		p_resources.maxImageSamples = 0;
		p_resources.maxVertexImageUniforms = 0;
		p_resources.maxTessControlImageUniforms = 0;
		p_resources.maxTessEvaluationImageUniforms = 0;
		p_resources.maxGeometryImageUniforms = 0;
		p_resources.maxFragmentImageUniforms = 8;
		p_resources.maxCombinedImageUniforms = 8;
		p_resources.maxGeometryTextureImageUnits = 16;
		p_resources.maxGeometryOutputVertices = 256;
		p_resources.maxGeometryTotalOutputComponents = 1024;
		p_resources.maxGeometryUniformComponents = 1024;
		p_resources.maxGeometryVaryingComponents = 64;
		p_resources.maxTessControlInputComponents = 128;
		p_resources.maxTessControlOutputComponents = 128;
		p_resources.maxTessControlTextureImageUnits = 16;
		p_resources.maxTessControlUniformComponents = 1024;
		p_resources.maxTessControlTotalOutputComponents = 4096;
		p_resources.maxTessEvaluationInputComponents = 128;
		p_resources.maxTessEvaluationOutputComponents = 128;
		p_resources.maxTessEvaluationTextureImageUnits = 16;
		p_resources.maxTessEvaluationUniformComponents = 1024;
		p_resources.maxTessPatchComponents = 120;
		p_resources.maxPatchVertices = 32;
		p_resources.maxTessGenLevel = 64;
		p_resources.maxViewports = 16;
		p_resources.maxVertexAtomicCounters = 0;
		p_resources.maxTessControlAtomicCounters = 0;
		p_resources.maxTessEvaluationAtomicCounters = 0;
		p_resources.maxGeometryAtomicCounters = 0;
		p_resources.maxFragmentAtomicCounters = 8;
		p_resources.maxCombinedAtomicCounters = 8;
		p_resources.maxAtomicCounterBindings = 1;
		p_resources.maxVertexAtomicCounterBuffers = 0;
		p_resources.maxTessControlAtomicCounterBuffers = 0;
		p_resources.maxTessEvaluationAtomicCounterBuffers = 0;
		p_resources.maxGeometryAtomicCounterBuffers = 0;
		p_resources.maxFragmentAtomicCounterBuffers = 1;
		p_resources.maxCombinedAtomicCounterBuffers = 1;
		p_resources.maxAtomicCounterBufferSize = 16384;
		p_resources.maxTransformFeedbackBuffers = 4;
		p_resources.maxTransformFeedbackInterleavedComponents = 64;
		p_resources.maxCullDistances = 8;
		p_resources.maxCombinedClipAndCullDistances = 8;
		p_resources.maxSamples = 4;
		p_resources.maxMeshOutputVerticesNV = 256;
		p_resources.maxMeshOutputPrimitivesNV = 512;
		p_resources.maxMeshWorkGroupSizeX_NV = 32;
		p_resources.maxMeshWorkGroupSizeY_NV = 1;
		p_resources.maxMeshWorkGroupSizeZ_NV = 1;
		p_resources.maxTaskWorkGroupSizeX_NV = 32;
		p_resources.maxTaskWorkGroupSizeY_NV = 1;
		p_resources.maxTaskWorkGroupSizeZ_NV = 1;
		p_resources.maxMeshViewCountNV = 4;
		p_resources.limits.nonInductiveForLoops = 1;
		p_resources.limits.whileLoops = 1;
		p_resources.limits.doWhileLoops = 1;
		p_resources.limits.generalUniformIndexing = 1;
		p_resources.limits.generalAttributeMatrixVectorIndexing = 1;
		p_resources.limits.generalVaryingIndexing = 1;
		p_resources.limits.generalSamplerIndexing = 1;
		p_resources.limits.generalVariableIndexing = 1;
		p_resources.limits.generalConstantMatrixVectorIndexing = 1;
	}

	EShLanguage SpirvHelper::findLanguage(const vk::ShaderStageFlagBits shader_type)
	{
		switch (shader_type)
		{
		case vk::ShaderStageFlagBits::eVertex:
			return (EShLangVertex);
		case vk::ShaderStageFlagBits::eTessellationControl:
			return (EShLangTessControl);
		case vk::ShaderStageFlagBits::eTessellationEvaluation:
			return (EShLangTessEvaluation);
		case vk::ShaderStageFlagBits::eGeometry:
			return (EShLangGeometry);
		case vk::ShaderStageFlagBits::eFragment:
			return (EShLangFragment);
		case vk::ShaderStageFlagBits::eCompute:
			return (EShLangCompute);
		default:
			return (EShLangVertex);
		}
	}

	bool SpirvHelper::GLSLtoSPV(const vk::ShaderStageFlagBits shader_type, const char* pshader, std::vector<unsigned int>& spirv)
	{
		EShLanguage stage = findLanguage(shader_type);
		glslang::TShader shader(stage);
		glslang::TProgram program;
		const char* shaderStrings[1];
		TBuiltInResource resources = {};
		initResources(resources);

		// Enable SPIR-V and Vulkan rules when parsing GLSL
		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

		shaderStrings[0] = pshader;
		shader.setStrings(shaderStrings, 1);

		if (!shader.parse(&resources, 100, false, messages)) {
			spk::cout << "Parsing: " << shader.getInfoLog() << std::endl;
			spk::cout << shader.getInfoDebugLog() << std::endl;
			return (false);
		}

		program.addShader(&shader);

		if (!program.link(messages)) {
			spk::cout << shader.getInfoLog() << std::endl;
			spk::cout << shader.getInfoDebugLog() << std::endl;
			fflush(stdout);
			return (false);
		}

		glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
		return (true);
	}
	
	vk::Format attributeFormat(const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration::Attribute& p_attribute)
	{
		if (p_attribute.type == spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float)
		{
			switch (p_attribute.format)
			{
			case 1:
				return (vk::Format::eR32Sfloat);
			case 2:
				return (vk::Format::eR32G32Sfloat);
			case 3:
				return (vk::Format::eR32G32B32Sfloat);
			case 4:
				return (vk::Format::eR32G32B32A32Sfloat);
			default:
				return (vk::Format::eUndefined);
			}
		}
		else if (p_attribute.type == spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int)
		{
			switch (p_attribute.format)
			{
			case 1:
				return (vk::Format::eR32Sint);
			case 2:
				return (vk::Format::eR32G32Sint);
			case 3:
				return (vk::Format::eR32G32B32Sint);
			case 4:
				return (vk::Format::eR32G32B32A32Sint);
			default:
				return (vk::Format::eUndefined);
			}
		}
		else if (p_attribute.type == spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt)
		{
			switch (p_attribute.format)
			{
			case 1:
				return (vk::Format::eR32Uint);
			case 2:
				return (vk::Format::eR32G32Uint);
			case 3:
				return (vk::Format::eR32G32B32Uint);
			case 4:
				return (vk::Format::eR32G32B32A32Uint);
			default:
				return (vk::Format::eUndefined);
			}
		}
		return (vk::Format::eUndefined);
	}
}