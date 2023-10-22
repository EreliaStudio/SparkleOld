#pragma once

#include <string>
#include "spk_system_define.hpp"

namespace spk
{
    /**
     * Converts a GLenum value to its corresponding wide string representation.
     * 
     * @param p_type The GLenum value to convert.
     * @return The wide string representation of the GLenum.
     */
    std::wstring convertGLEnumToWString(const GLenum& p_type);

    /**
     * Checks for OpenGL errors and logs them along with a provided message.
     * 
     * @param msg The message to log along with any OpenGL errors.
     */
    void checkOpengl(const std::wstring& msg);
}

/**
 * Macro for debugging OpenGL. Calls spk::checkOpengl with class, method, and line number information.
 */
#define GPU_DEBUG_LINE() spk::checkOpengl(__CLASS__ + L"::" + __METHOD__ + L"::" + std::to_wstring(__LINE__))
