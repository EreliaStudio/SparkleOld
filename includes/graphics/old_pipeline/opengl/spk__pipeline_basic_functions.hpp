#pragma once

#include <string>
#include "spk_system_define.hpp"
#include "graphics/pipeline/spk_abstract_pipeline.hpp"
#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void compileShaderModule(GLuint p_shaderIndex, std::string p_shaderCode);
	void compileProgram(GLuint p_programID, GLuint p_vertexID, GLuint p_fragmentID);

	std::wstring convertGLEnumToWString(const GLenum& p_type);
	void checkOpengl(const std::wstring& msg);
}

#define GPU_DEBUG_LINE() spk::checkOpengl(__CLASS__ + L"::" + __METHOD__ + L"::" + std::to_wstring(__LINE__))