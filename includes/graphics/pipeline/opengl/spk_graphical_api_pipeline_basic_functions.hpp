#pragma once

#include <string>
#include "spk_system_define.hpp"
#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	GLenum convertModeToGLenum(const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration::Mode &p_input);
	GLenum convertTypeToGLenum(const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration::Attribute::Type &p_input);
	void compileShaderModule(GLuint p_shaderIndex, std::string p_shaderCode);
	void compileProgram(GLuint p_programID, GLuint p_vertexID, GLuint p_fragmentID);

	std::wstring convertGLEnumToWString(const GLenum& p_type);
	void checkOpengl(const std::wstring& msg);
}