#pragma once

#include "graphics/pipeline/spk_shader_module.hpp"

namespace spk
{
    /**
     *  @brief Extern declaration for the Vertex Shader Module of the Box Component.
     *
     *  This shader module is intended to handle the vertex shading logic
     *  specific to rendering a Box Component in the graphics pipeline.
     */
    extern spk::ShaderModule boxComponentVertexShaderModule;

    /**
     *  @brief Extern declaration for the Fragment Shader Module of the Box Component.
     *
     *  This shader module is intended to handle the fragment shading logic
     *  specific to rendering a Box Component in the graphics pipeline.
     */
    extern spk::ShaderModule boxComponentFragmentShaderModule;
}