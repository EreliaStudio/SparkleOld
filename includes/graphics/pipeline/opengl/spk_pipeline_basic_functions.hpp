#pragma once

#include <string>
#include "spk_system_define.hpp"

namespace spk
{
	std::wstring convertGLEnumToWString(const GLenum& p_type);
	void checkOpengl(const std::wstring& msg);
}

#define GPU_DEBUG_LINE() spk::checkOpengl(__CLASS__ + L"::" + __METHOD__ + L"::" + std::to_wstring(__LINE__))